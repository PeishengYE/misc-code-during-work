#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <mysql/mysql.h>

#define IPADDRESS			"127.0.0.1"		//listen on 127.0.0.1
#define DBADDRESS			"127.0.0.1"		//mysql server address
#define DBUSER				"root"			//mysql database user
#define DBPWD				"toor"			//mysql database password
#define PORT				8787		  	
#define MAXSIZE				2048			//every message should use this size, both client and server.
#define LISTENQ				20				//the sencond parameter of listen()
#define FDSIZE				11000			//parameter of epoll_create, although ignored since linux 2.6.8, make it bigger than 10000.
#define EPOLLEVENTS			1000			//maxevents of epoll_waite.
#define WORKER_THREAD_CNT	20				//number of threads that read data from socket. 
#define PARSER_THREAD_CNT	20				//number of threads that parse data from buffer and query mysql.
#define TIMELEN				30				//max len for logtime string.
#define BUFSIZE				1024*1024*1024	//size of buffer for saving received logs.
//#define BUFSIZE			256*1024		//for test reusing the buffer.
#define PARSE_N				100				//each time a parser parses at maximum PARSE_N logs.
#define QUERYSIZE			PARSE_N*1024	//max size of the query string.
#define GARBAGESIZE			102400			//size of garbage buffer, for droping data.

typedef struct task {
	int epollfd;
	int fd;
	struct task *next;
} task_t;

typedef struct ipstruct {
	unsigned char ip1;
	unsigned char ip2;
	unsigned char ip3;
	unsigned char ip4;	
}ipstru;

typedef struct logstruct { 					//for saving information of one log.
	char time [TIMELEN];
	union {
	int srcip;
	ipstru sipstru;
	};	
	union {
		int destip;
		ipstru dipstru;
	};
	int volume;
	char msg [801];
}logstru;

MYSQL *db_conn = NULL;
task_t *task_head = NULL, *task_tail = NULL;
pthread_mutex_t mtx;						//lock for tasks;
pthread_mutex_t write_mtx;					//lock for write data to buffer;
pthread_mutex_t parse_mtx;					//lock for parser read data from buffer;
pthread_mutex_t query_mtx;					//lock for query mysql;
pthread_cond_t cond;
pthread_t read_tid[WORKER_THREAD_CNT];		//for threads that read sockets;
pthread_t parse_tid[PARSER_THREAD_CNT];		//for parse threads

char buf_begin[BUFSIZE];                	//buffer for saving received logs.
char * buf_end = buf_begin + BUFSIZE;
char * data_begin = buf_begin;          	//beginning of received logs.
char * data_end = buf_begin;            	//ending of received logs.
char * data_farpoint = buf_begin + BUFSIZE;	//sometimes, the data can not reach the very end of the buffer, and start again at the buf_begin.
																									//for example, when MAXSIZE = 2049.
char garbage_buf[GARBAGESIZE];					//just for drop data;

static int socket_bind(const char* ip,int port);
static void do_epoll(int listenfd);
static void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd);
static void handle_accept(int epollfd,int listenfd);
static void* do_read(void *args);
static void add_event(int epollfd,int fd,int state);
//static void delete_event(int epollfd,int fd,int state);
static void set_nonblocking(int sock);
static void init_thread_pool();
static int getip(ipstru * pipstru, char logstr[], int len);
static int parseLog(char *logstr, logstru lstru[], int lognum, int * idx, int maxlen);
static void printLog(logstru prnstru);
static MYSQL* db_init(char *server, char *user, char *passwd, char *database);
static MYSQL_RES* db_query(MYSQL *conn, char *query);
static void db_close(MYSQL *conn);
static void db_writeLog();
static void* do_parse(void *args);

int main(int argc,char *argv[])
{
	db_conn = db_init(DBADDRESS, DBUSER, DBPWD, "TrafficDB");
	if (!db_conn) {
		return 0;
	}
	  
	init_thread_pool();
	int  listenfd;
	listenfd = socket_bind(IPADDRESS,PORT);
	listen(listenfd,LISTENQ);
	printf("Start the server at %s on port %d\n", IPADDRESS, PORT);
	do_epoll(listenfd);
	return 0;
}

static void init_thread_pool()
{
    pthread_mutex_init(&mtx,NULL);
    pthread_mutex_init(&write_mtx,NULL);
    pthread_mutex_init(&parse_mtx,NULL);
    pthread_mutex_init(&query_mtx,NULL);
	pthread_cond_init(&cond,NULL);

	int j = 0;
	for (; j < WORKER_THREAD_CNT; j++) {
		pthread_create(&read_tid[j], NULL, do_read, NULL);
	}
	int i = 0;
	for (; i < PARSER_THREAD_CNT; i++) {
		pthread_create(&parse_tid[i], NULL, do_parse, NULL);
	}
}

static int socket_bind(const char* ip,int port)
{
	int  listenfd;
	struct sockaddr_in servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if (listenfd == -1) {
		perror("socket error:");
		exit(1);
	}
	set_nonblocking(listenfd);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1) {
		perror("bind error: ");
		exit(1);
	}
	return listenfd;
}

static void do_epoll(int listenfd)
{
	int epollfd;
	struct epoll_event events[EPOLLEVENTS];
	int ret;
	epollfd = epoll_create(FDSIZE);
	add_event(epollfd,listenfd,EPOLLIN|EPOLLET);
	for ( ; ; ) {
		ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
		handle_events(epollfd,events,ret,listenfd);
	}
	close(epollfd);
}

static void
handle_events(int epollfd,struct epoll_event *events,int num,int listenfd)
{
	int i;
	int fd;
	for (i = 0;i < num;i++) {
		fd = events[i].data.fd;
		if ((fd == listenfd) &&(events[i].events & EPOLLIN)) {
			handle_accept(epollfd,listenfd);
		} else if (events[i].events & EPOLLIN) {
			task_t *new_task = (task_t *)malloc(sizeof(task_t));
			if (!new_task) {
				perror("new task allocation failure:");
				return;
			}
			new_task->epollfd = epollfd;
			new_task->fd = fd;
			new_task->next = NULL;
			pthread_mutex_lock(&mtx);
			if (task_head == NULL) {
				task_head = new_task;
				task_tail = new_task;
			} else {
				task_tail->next = new_task;
				task_tail = new_task;
			}
			pthread_cond_broadcast(&cond);
			pthread_mutex_unlock(&mtx);
		}
	}
}

static void handle_accept(int epollfd,int listenfd)
{
	int clifd;
	struct sockaddr_in cliaddr;
	socklen_t  cliaddrlen = sizeof(cliaddr);
	clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
	if (clifd == -1) {
		perror("accpet error:");
	} else {
		printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
		set_nonblocking(clifd);
		add_event(epollfd,clifd,EPOLLIN|EPOLLET);
	}
}

static int read_socket(int fd, char * buf_point) //read data from socket, write to buffer, start from buf_point.
{
	int nread, r;
	nread = read(fd,buf_point,MAXSIZE);

    if (nread <= 0) {
    	return -1;
	} else {    	
		while(nread < MAXSIZE) {
    		r = read(fd,buf_point,MAXSIZE-nread);
    		if (r > 0) {
    			nread += r;
    		} else {
    			break;
    		}
		}
		data_end = buf_point + nread;
        
		//ack client with "OK".
		char ackbuf[2] = "OK";
		int ret = send(fd, ackbuf, 2, 0);
	}
	return nread;
}

static int drop_data(fd) //drop all data of fd;
{
	int r, drop;
	
	//printf("*******************  DROP  *********************\n");
	r = read(fd, garbage_buf, GARBAGESIZE);
	drop = r;
	while (r > 0) {
		r = read(fd, garbage_buf, GARBAGESIZE);
		drop = drop + r;
	}
	return drop;
}

static int write_gbuf(int fd)
{
	int nread = 1;
    
	if (data_begin > data_end)	//data_end has reached the buf_end and go back to the beginning part of the buffer, while data_begin is still on the way to buf_end.
								//the area between data_end and data_begin is free to write.
								// buf_end---(used buffer)---data_begin===(free bufffer)===data_end---(used buffer)---buf_begain.
	{
		if (data_begin - data_end > MAXSIZE) {	//enough buffer for saving received data.
			nread = read_socket(fd,data_end);
		} else {	//no more free buffer for saving the log, drop received data.
        	nread = drop_data(fd);
        }
    } else {	// buf_end===(free buffer)===data_end---(used buffer)---data_begin===(free buffer)===buf_begin.
    	if (buf_end - data_end >= MAXSIZE) {	//the area between data_end and buf_end is big enough to write.
    		nread = read_socket(fd,data_end);
    	} else {	//data_end reach the end of buffer, should reuse the beginning part of the buffer.
    		data_farpoint = data_end;
    		if (data_begin - buf_begin >= MAXSIZE) {	//the beginning part of the buffer can be reused now.
    			nread = read_socket(fd,buf_begin);
    		} else {	//no more free buffer for saving the log, drop received data.
    			nread = drop_data(fd);
    		}
    	}
	}
    return nread;
}

static void* do_read(void *args) //read socket thread.
{
	int fd, epollfd, nrcv;
	while (1) {
		pthread_mutex_lock(&mtx);

		while (task_head == NULL) {
			pthread_cond_wait(&cond,&mtx);
		}

		fd = task_head->fd;
		epollfd = task_head->epollfd;
    
		task_t *tmp = task_head;
		task_head = task_head->next;
		free(tmp);
		pthread_mutex_unlock(&mtx);

		pthread_mutex_lock(&write_mtx);
		//method 1.
		//every time, read all possible messages from fd. but, these messages all triggered their own event.
		//so other threads that response to these events, will not read the message that triggered the event.
		nrcv = 1;
		while (nrcv > 0) {
			nrcv = write_gbuf(fd);
		}
		
		//method 2.
		//nrcv = write_gbuf(fd); //every time, only read one message (MAXSIZE) from the socket. 
								//that is every thread get the message that triggered the event to which this thread response.
		pthread_mutex_unlock(&write_mtx);
	}
}

static void add_event(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
/*
static void delete_event(int epollfd,int fd,int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
*/
static void set_nonblocking(int sock)
{
	int opts;
	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl GETFL error:");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl SETFL error:");
		exit(1);
	}
}

static int getip(ipstru * pipstru, char logstr[], int len) //parse ip string.
{
	int i = 0;
	pipstru->ip1 = (unsigned char)atoi(logstr);
	while (i < len && logstr[i] != '.') {
		i++;
	}
	if (i < len) {
		i++;
		pipstru->ip2 = (unsigned char)atoi(&(logstr[i]));
	} else {
		return -1;
	}

	while (i < len && logstr[i] != '.') {
		i++;
	}
	if (i < len) {
		i++;
		pipstru->ip3 = (unsigned char)atoi(&(logstr[i]));
	} else {
		return -1;
	}

	while (i < len && logstr[i] != '.') {
		i++;
	}
	if (i < len) {
		i++;
		pipstru->ip4 = (unsigned char)atoi(&(logstr[i]));
	} else {
		return -1;
	}

	return 0;
}

static int parse_log(logstru * lstru) //parse log message.
{
    int maxlen = 0;		//the maximum number of bytes can be parsed. 
    int buf_idx = 0;	//index used to go throught buffer.
    int lstru_idx = 0;	//index of structure array used for saving parsed information.
    int tmpi = 0;

	if (data_end > data_begin) { //buf_end===(free buffer)===data_end---(used buffer)---data_begin===(free buffer)===buf_begin.
        maxlen = data_end - data_begin;
    } else {	//buf_end---(used buffer)---data_begin===(free bufffer)===data_end---(used buffer)---buf_begain.
        maxlen = data_farpoint - data_begin; //it is the length of data between buf_end and data_begin.
        // when buffer size is not divisible by MAXSIZE, for example, MAXSIZE=1029, data can not reach the very end of buffer (buff_end).
        // in this case, data_farpoint stand for the farest end of data.
    }
    
    while (buf_idx < maxlen && lstru_idx < PARSE_N) {
        switch (data_begin[buf_idx]) {
            case '\n': // end of a log.
                lstru_idx++;
                buf_idx++;
                break;
            case 'l':
                if (strncmp(&(data_begin[buf_idx]), "logtime", 7) == 0)
                {
                    buf_idx += 8;
                    while (buf_idx < maxlen && isdigit(data_begin[buf_idx]) == 0 && data_begin[buf_idx] != '\n') {
                    	//logtime's value is always started by digit, ended with ",".
                        buf_idx++;
                    }
                    if (buf_idx < maxlen && data_begin[buf_idx] != '\n') {
                        tmpi = 0;
                        while (buf_idx < maxlen && data_begin[buf_idx] != ',' && tmpi < TIMELEN-1) {
                            lstru[lstru_idx].time[tmpi] = data_begin[buf_idx];
                            tmpi++;
                            buf_idx++;
                        }
                        lstru[lstru_idx].time[tmpi]='\0';
                    }
                } else {
                    buf_idx++;
                }
                break;
            case 's':
                if (strncmp(&(data_begin[buf_idx]), "srcip", 5) == 0) {
                    buf_idx += 6;
                    while (buf_idx < maxlen && isdigit(data_begin[buf_idx]) == 0 && data_begin[buf_idx] != '\n') {
                    	//srcip's value is always started by digit, ended with ",".
                        buf_idx++;
                    }
                    if (buf_idx < maxlen && data_begin[buf_idx] != '\n') {
                        tmpi = buf_idx;
                        while (buf_idx < maxlen && data_begin[buf_idx] != ',' && data_begin[buf_idx] != '\n') {
                            buf_idx++;
                        }
                        if (buf_idx < maxlen) {
                            getip(&(lstru[lstru_idx].sipstru), &(data_begin[tmpi]), buf_idx-tmpi);
                        }
                    }
                } else {
                    buf_idx++;
                }
                break;
            case 'd':
                if (strncmp(&(data_begin[buf_idx]), "destip", 6) == 0) {
                    buf_idx += 7;
                    while (buf_idx < maxlen && isdigit(data_begin[buf_idx]) == 0 && data_begin[buf_idx] != '\n') {
                    	//destip's value is always started by digit, ended with ",".
                        buf_idx++;
                    }
                    if (buf_idx < maxlen && data_begin[buf_idx] != '\n') {
                        tmpi = buf_idx;
                        while (buf_idx < maxlen && data_begin[buf_idx] != ',' && data_begin[buf_idx] != '\n') {
                            buf_idx++;
                        }
                        if (buf_idx < maxlen) {
                            getip(&(lstru[lstru_idx].dipstru), &(data_begin[tmpi]), buf_idx-tmpi);
                        }
                    }
                } else {
                    buf_idx++;
                }
                break;
            case 'v':
                if (strncmp(&(data_begin[buf_idx]), "volume", 6) == 0) {
                    buf_idx += 7;
                    while (buf_idx < maxlen && isdigit(data_begin[buf_idx]) == 0 && data_begin[buf_idx] != '\n') {
                    	//volume is always a number.
                        buf_idx++;
                    }
                    if (buf_idx < maxlen && data_begin[buf_idx] != '\n') {
                        lstru[lstru_idx].volume = atoi(&(data_begin[buf_idx]));
                        buf_idx++;
                    }
                } else {
                    buf_idx++;
                }
                break;
            case 'm':
                if (strncmp(&(data_begin[buf_idx]), "msg", 3) == 0) {
                    buf_idx += 4;
                    while (buf_idx < maxlen && data_begin[buf_idx] != '\"' && data_begin[buf_idx] != '\n') {
                    	//msg's value is always started by '\"'.
                        buf_idx++;
                    }
                    if (buf_idx < maxlen && data_begin[buf_idx] == '\"') {
                        tmpi = 0;
                        buf_idx++;
                        while (buf_idx < maxlen && data_begin[buf_idx] != '\"' && tmpi < 800) {
                        	// and ended with '\"'.
                            lstru[lstru_idx].msg[tmpi] = data_begin[buf_idx];
                            tmpi++;
                            buf_idx++;
                        }
                        lstru[lstru_idx].msg[tmpi] = '\0';
                    }
                } else {
                    buf_idx++;
                }
                break;
            default:
                buf_idx++;
                break;
        }
    }

	//set data_begin to new position.
    data_begin = data_begin + buf_idx;
    if (data_begin >= data_farpoint && data_begin != data_end) { 
    	// if data_begin reaches the end of buffer, and there's more data available, data_begin should go back to buf_begin.
        data_begin = buf_begin;
    }
    return lstru_idx;
}

static MYSQL* db_init(char *server, char *user, char *passwd, char *database)
{
	MYSQL *conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, passwd, database, 0, NULL, 0)) {
		printf("Failure in db connection: %s\n", mysql_error(conn));
		return NULL;
	}

	return conn;
}

static MYSQL_RES* db_query(MYSQL *conn, char *query)
{
	int ret = mysql_query(conn, query);
	
	if (ret) {
		printf("Failure in db query: %s\n", mysql_error(conn));
		return NULL;
	} else {
		MYSQL_RES *res = mysql_store_result(conn);
		return res;
	}
	
}

static void db_close(MYSQL *conn)
{
	mysql_close(conn);
}

static void db_writeLog(logstru * lstru, int lnum) //generate the query string, and query mysql.
{
	char sql_query[QUERYSIZE];
	int i = 0;
	memset(sql_query, 0, QUERYSIZE);
	//Here use 'volume', if it is really 'volumn', just change it.
	sprintf(sql_query,
             "INSERT INTO Traffic (logtime, srcip, destip, volume, msg) VALUES ('%s', %d, %d, %d, '%s')",lstru[0].time,
             lstru[0].srcip, lstru[0].destip, lstru[0].volume, lstru[0].msg);
	
    for (i = 1; i < lnum; i++) {
        sprintf(sql_query,
                 "%s,('%s', %d, %d, %d, '%s')",sql_query, lstru[i].time,
                 lstru[i].srcip, lstru[i].destip, lstru[i].volume, lstru[i].msg);
    }
    
    //printf("%s\n",sql_query);
	pthread_mutex_lock(&query_mtx);
    MYSQL_RES *res = db_query(db_conn, sql_query);
    unsigned int num_rows = mysql_affected_rows(db_conn);
    if (num_rows <= 0)
    {
        printf("[FUNC parse_check_result] MYSQL INSERT operation failure, rnt: %d\n", num_rows);
    }
	pthread_mutex_unlock(&query_mtx);
}

static void* do_parse (void *args) //parser thread.
{
	int num_parsed;
    logstru lstru[PARSE_N];
    
    while (1) {
    	num_parsed = 0;
    	pthread_mutex_lock(&parse_mtx);
    	if (data_begin != data_end) { //there is data available.
    		num_parsed = parse_log(lstru);
    	}
    	pthread_mutex_unlock(&parse_mtx);
    	
        if (num_parsed > 0) {
            db_writeLog(lstru, num_parsed); //send data to mysql.
        } else {
        	sleep(1);
        }
    }
}

