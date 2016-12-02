#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct circular_buffer
{
	void *start;
	void *end;
	size_t buffer_size;
	int full_flag;
	void *head;
	void *tail;

}recving_buffer;


typedef int BOOL;
#define TRUE   1
#define FALSE  0

#define PC_TEST
#ifdef PC_TEST
#define db_debug(fmt, ...) \  
	do { fprintf(stdout, "%s()>> %d::", __FUNCTION__,  __LINE__); \ 
		fprintf(stdout, fmt, ##__VA_ARGS__);} while (0)
#else
#define db_debug(fmt, ...)   available_buffer_size
#endif 

#define BUFFER_SIZE 0x20

		int finished_flag = FALSE;


void rcving_buffer_init( recving_buffer *rb, size_t sz )
{
	rb->start = malloc(sz);
	if (rb->start == NULL)
		return;
	rb->end  = rb->start + sz -1 ;
	rb->head = rb->start;
	rb->tail = rb->start;
	
	rb->buffer_size = sz ;
	rb->full_flag = FALSE; /* set to empty when head == tail */
	db_debug("end = %d", (int)(rb->end - rb->start));
}

int check_data_size( recving_buffer *rb, void *tail, void *head)
{
    int data_size, remain_data_size;
	db_debug("tail = 0x%x, head = 0x%x\n", rb->tail , rb->head );
	db_debug("tail = %d, head = %d\n", rb->tail - rb->start, rb->head - rb->start);

    if ((head == tail)){
		if(rb->full_flag == TRUE){
			data_size = rb->buffer_size; 
	  		db_debug("head == tail:: FULL \n");   
		}else{
			data_size = 0; 
	  		db_debug("head == tail:: EMPTY \n");   
		}

	  return data_size;
  }


  if(head > tail){

	  data_size = head -tail;
	  db_debug("> data_size == %d \n", data_size);   
  	  return data_size;
  }

  if(head < tail){
	remain_data_size = rb->end - tail + 1;
	data_size = head - rb->start + remain_data_size;
	db_debug("< data_size == %d \n", data_size);   
	return data_size;
  }
}

int check_available_buffer_size( recving_buffer *rb, void *tail, void *head)
{
	int data_size, ret;
	data_size = check_data_size(rb, tail, head);
	ret = rb->buffer_size - data_size;
	 db_debug("return: available_buffer_size = %d \n", ret);   
	return ret;

}

BOOL is_buffer_empty(recving_buffer *rb, void *tail, void *head)
{
	int data_size;
	data_size = check_data_size(rb, tail, head);
	if (data_size == 0 )
		return TRUE;
	else
		return FALSE;

}

BOOL is_buffer_full(recving_buffer *rb, void *tail, void *head)
{
	int data_size;
	data_size = check_data_size(rb, tail, head);
	if (data_size == rb->buffer_size )
		return TRUE;
	else
		return FALSE;

}

/*
 * this is on the producer side,
 * it write data to the buffer
 * it only change the HEAD, we don't change tail
 *
 * the parameter tail is to prevent the consumer side changing the tail
 * make the calculation inconsistent
 *
 * NOTE : make sure that the writing_size  <= available_buffer_size 
 * */
void write_buffer(recving_buffer *rb, void *tail, void *mem, size_t writing_size) 
{
			db_debug("**>>** %d\n", writing_size);   
	/* when the tail is behind the head */
	if (tail <= rb->head){

		int remain_size = rb->end - rb->head +1; 

		if(writing_size > remain_size){
			memcpy(rb->head, mem, remain_size);
			memcpy(rb->start , (mem+ remain_size), writing_size - remain_size);
			rb->head = rb->start + writing_size - remain_size;
		}else{
			memcpy(rb->head, mem, writing_size);
			rb->head = rb->head + writing_size;

			if (rb->head == rb->end + 1){
				rb->head = rb->start;
			}
			if(rb->head > (rb->end + 1))
	  			db_debug("critical issue in write_buffer with %d\n", writing_size);   
		}

	}else {
		memcpy(rb->head, mem, writing_size);
		rb->head = rb->head + writing_size;

	}
	
	if (rb->head == tail)
		rb->full_flag = TRUE;

}
/*
 * this is on the consumer side, 
 * it read data from the buffer 
 * it only change the TAIL, we don't change head'
 *
 * the parameter head is to prevent the producer side changing the head
 * make the calculation inconsistent'
 *
 * NOTE : make sure that the copy_size <= available_data_size
 * */
void read_and_remove_from_buffer(recving_buffer *rb,void *head, size_t reading_size, void*mem)
{
			db_debug("**<<** %d\n", reading_size);   
	/* the tail is behind head */
	if(head > rb->tail){
		/* NOTE: this can make head == tail */
		memcpy(mem, rb->tail, reading_size);
		rb->tail = rb->tail + reading_size;

		/* the tail is before head */
		/* tail == head means the buffer is full now 
		 * it seems we should make same operation
		 * */
	}else {
			int remain_size = rb->end - rb->tail + 1;

			if (reading_size > remain_size ){
				memcpy(mem, rb->tail, remain_size);
				memcpy((mem + remain_size), rb->start,  reading_size - remain_size);
				rb->tail = rb->start + reading_size - remain_size;

			}else{
				memcpy(mem, rb->tail, reading_size);

				/* change well the tail pointer, it may need be point to start */
				if ( rb->tail + reading_size == rb->end +1 ){
					rb->tail = rb->start;

				}else if(rb->tail + reading_size < rb->end +1) {
					rb->tail = rb->tail + reading_size;
				}else{
	  				db_debug("critical issue in read_and_remove_from_buffer  \n");   
				}

		}

		}

	if (rb->tail == head)
		rb->full_flag = FALSE;

}

/* testing code */
#define FILENAME_READING "file_input.jpg"
#define FILENAME_WRITING "file_output.jpg"

void read_buffer_write_file( recving_buffer *rb, FILE *fp, size_t sz )
{
	char * mem;
	/* allocating memory */
	mem = (char *)malloc(sz);

	/* reading buffer */
	read_and_remove_from_buffer(rb, rb->head, sz, mem);

	/* write buffer */
	fwrite(mem, 1, sz, fp); 

	/* free memory */
	free(mem);

}

int  read_file_write_buffer( recving_buffer *rb, FILE *fp, size_t sz)
{
	char * mem;
	int ret;
	/* allocating memory */
	mem = (char *)malloc(sz);

	/* reading from file */
	ret = fread(mem, 1, sz, fp);

	/* writing to buffer */
	//write_buffer(recving_buffer *rb, void *tail, void *mem, size_t writing_size) 
	//write_buffer(rb, rb->tail, mem, sz); 
	if(ret > 0)
	write_buffer(rb, rb->tail, mem, ret); 

	/* free memory */
	free(mem);

	return ret;

}

void * writing_thread_func(void *arg)
{
	FILE *fp_in;
	int writing_number, available_buffer_size, nread;
	recving_buffer *rb = (recving_buffer *)arg;
	/* open the input file */

	while(1){
		/* yield to reading_thread */
		sched_yield();

		/* generating a random number */
		writing_number = rand()%(BUFFER_SIZE-1) + 1;

		/* check if buffer is available */
		available_buffer_size = check_available_buffer_size( rb, rb->tail, rb->head);
		if(writing_number <= available_buffer_size){
		/* read file and write buffer */
			nread = read_file_write_buffer( rb, fp_in, writing_number);
		/* check the return reading value */
			if (nread <= 0){
				break;
			}
		}
	}

	/* set the finishing flag */
	finished_flag = TRUE;

	/* close file */
	 fclose(fp_in);

}

void * reading_thread_func(void *arg)
{
	FILE *fp_out;
	int reading_number, data_size;
	recving_buffer *rb = (recving_buffer *)arg;
	/* open the output file */

	while(1){
		/* yield for the writing thread */
		sched_yield();

		/*  */
		reading_number = rand()%(BUFFER_SIZE-1) + 1;
		 data_size = check_data_size( rb, rb->tail, rb->head);

		 //if(reading_number <= data_size){
		 if(reading_number <= data_size){
			read_buffer_write_file( rb, fp_out, reading_number );

		 }

		 if(finished_flag == TRUE){
			 break;
		 }

	}

	/**/
		 data_size = check_data_size( rb, rb->tail, rb->head);

		 if (data_size >0)
			read_buffer_write_file( rb, fp_out, data_size );

		 fclose(fp_out);

}

#define MULTI_THREAD 
int main()
{
	size_t sz = BUFFER_SIZE ;
	FILE *fp_in, *fp_out;
	int available_buffer_size, data_size;

	int reading_number, nread; 
	int writing_number, nwrite; 

	pthread_t reading_thread;
	pthread_t writing_thread;

	recving_buffer rbs;
	recving_buffer *rb = &rbs;


	rcving_buffer_init( rb,  sz);
#ifndef MULTI_THREAD 
	/* 
	 *
	 * open a reading file
	 * open a writing file 
	 * */
	
	fp_in = fopen(FILENAME_READING, "r+");
	fp_out = fopen(FILENAME_WRITING, "w+");


	while(1){
		writing_number = rand()%(sz-1) + 1;

		available_buffer_size = check_available_buffer_size( rb, rb->tail, rb->head);

		if(writing_number <= available_buffer_size){
			nread = read_file_write_buffer( rb, fp_in, writing_number);
			if (nread <= 0){

				break;
			}
		}

		reading_number = rand()%(sz-1) + 1;
		 data_size = check_data_size( rb, rb->tail, rb->head);

		 //if(reading_number <= data_size){
		 if(reading_number <= data_size){
			read_buffer_write_file( rb, fp_out, reading_number );

		 }

	}

	  	db_debug("finished looping, last step : clear the buffer\n");   
	/*
	 * read the end of data in the buffer
	 * write to the writing file 
	 * */
		 data_size = check_data_size( rb, rb->tail, rb->head);
		 if (data_size >0)
			read_buffer_write_file( rb, fp_out, data_size );

	/*
	 * close file 
	 * */
		 fclose(fp_in);
		 fclose(fp_out);
#else
		 pthread_create(&writing_thread, NULL, writing_thread_func, rb);
		 pthread_create(&reading_thread, NULL, reading_thread_func, rb);

		 pthread_join(writing_thread);
		 pthread_join(reading_thread);

#endif



}



