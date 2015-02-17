/*
 * March 21st, AMD interview coding Question 2
 * creat two thread, open a file in a thread and pass the buffer to other thread for writing.
 * comparing the two files at the end
 *
 * requirement:  the copy buf size is random and under 100
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>

#define BUF_SIZE 100
#define COMP_BUF_SIZE 1024
#define FAILED 1
#define PASS 0 
#define YEP_DEBUG 

FILE *fp1,*fp2;
int file_size;
char buf[BUF_SIZE];
volatile char read_size=0;
volatile char buf_ready =0;
volatile char end_file =0;

pthread_mutex_t pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pthread_cond = PTHREAD_COND_INITIALIZER;

int buf_compare(char *buf1, char *buf2, int size){
	int i;
	for(i=0;i<size;i++){
		if(buf1[i] != buf2[i]){
			printf("Failed \n");
			return FAILED;
		}
	}

	return PASS;
}

/* return 1 for error */
/* return 0 for success */
int compare(FILE *file1, FILE *file2, int file_size)
{
	printf(">>compare\n");
	int readp=0,rest = 0, nread1 =0, nread2 =0;
	char buf1[COMP_BUF_SIZE];
	char buf2[COMP_BUF_SIZE];

	/* if file size is not equal, goto failed */

	while(1){

		if(nread1 = fread(buf1,COMP_BUF_SIZE,1,file1) !=1){
			printf("fead file1 failed\n");
			break;
		}
		if(nread2 = fread(buf2,COMP_BUF_SIZE,1,file2) !=1){
			printf("fead file2 failed\n");
			break;
		}
			
		readp += COMP_BUF_SIZE;

		if( buf_compare(buf1, buf2, COMP_BUF_SIZE) == FAILED){
			printf("FAILED\n");
			return 1;
		}

		if((file_size - readp) < COMP_BUF_SIZE){
			break;
		}

	}

	/* test only */
	if(feof(file1))
		printf("end of file1 \n");
	if(feof(file2))
		printf("end of file2 \n");
	/* end test only */

	rest = (file_size - readp);
	if (rest > COMP_BUF_SIZE){
			printf("FAILED on file size:  rest = %d \n", rest);
			return FAILED;
	}
		

	if(nread1 = fread(buf1,rest,1,file1) != 1)
		printf("file1 failed \n");
	if(nread2 = fread(buf2,rest,1,file2) !=1)
		printf("file2 failed \n");

	/* reach the end of file */	
	/* pay special attention to the end of file 
	 * only when the next read, the file pointer will be at the end of file 
	 * */


	
	/*compare the rest buf*/
	if(buf_compare(buf1, buf2, rest) == FAILED){
				printf("FAILED\n");
				return FAILED;
	}else{
				printf("PASS\n");

	}
		printf("<<compare\n");
		return PASS;
}

 


void * thread_write(void *arg)
{
	int rp = 0,rest,i;
	int nWrite =0 ;

	

	printf(">>write_thread\n");

	/* write the buf */

	pthread_mutex_lock(&pthread_mutex);
	while(1){

		while(( buf_ready == 0)){
			//printf("write_thread: wait\n");
			pthread_cond_wait(&pthread_cond, &pthread_mutex);
		}


			if(buf_ready == 1){
					//printf("write_thread: try to write %d\n",read_size);
					fwrite(buf,read_size,1,fp2);
					nWrite += read_size;
					buf_ready = 0;
			}

			if(end_file == 1 ) {
				//printf("write_thread: file end reached \n");
				break;
			}
	}


			pthread_mutex_unlock(&pthread_mutex);
			printf("write_thread: total write = %d\n",nWrite);
	return NULL;
}


int main(int argc, char * argv[])
{
	int 	i,n,rest,nread=0,rp=0,status;
	struct stat file_stat;



	/* open files */

	if((fp1 = fopen(argv[1],"r+")) == NULL)
		perror("fopen error");

	if((fp2 = fopen(argv[2],"w+")) == NULL)
		perror("fopen error");
	
	/* get file size */
	if(stat(argv[1],&file_stat) != 0)
		perror("stat error");

	printf("file_size   = %d\n",file_stat.st_size);
	file_size = file_stat.st_size;

	nread = fread(buf,read_size,1,fp1);
	
	while(nread == 1){
		fwrite(buf,read_size,1,fp2);
		nread = fread(buf,read_size,1,fp1);
	}

	/*
	 * when the last reading, it cannot get read size data, 
	 * how should I do for this
	 * */

	
}
