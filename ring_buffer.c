#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct ring_buffer{
	uint8_t *start;
	uint8_t *end;
	uint8_t *head;
	uint8_t *tail;
	uint8_t full_flag;
	uint32_t size;
} rb;

rb *p_rb = NULL;
#define BUFFER_FULL 8
#define BUFFER_EMPTY  4

void init_rb( int size)
{

	p_rb = (rb *)malloc(sizeof (rb));
	p_rb->start = (uint8_t *) malloc(size);
	p_rb->end = p_rb->start + size - 1;
	p_rb->head = p_rb->start;
	p_rb->tail = p_rb->start;
	p_rb->size = size;
	p_rb->full_flag = BUFFER_EMPTY;
}

void write_rb( uint8_t *mem, int mem_size)
{
	uint8_t *m_head, *m_tail;
	uint32_t remain_size;
	m_head = p_rb->head;
	m_tail = p_rb->tail;
	/**/
	remain_size = p_rb->end - p_rb->head + 1;
	if(m_head > m_tail){
		if(mem_size > remain_size ){
				memcpy(m_head,mem, remain_size);
				memcpy(p_rb->start, mem + remain_size, mem_size - remain_size);
				p_rb->head = p_rb->start + mem_size - remain_size;
		}else{
			memcpy(p_rb->head, mem, mem_size);
			p_rb->head = p_rb->head + mem_size;

			if(p_rb->head = p_rb->end + 1){
				p_rb->head = p_rb->start;
			}

		}

	}else{
		/* tail >= head */
		memcpy(p_rb->head, mem, mem_size);
		p_rb->head = p_rb->head + mem_size;

	}

	if(p_rb->head == p_rb->tail){
		p_rb->full_flag = BUFFER_FULL;

	}


}

void reading_rb(uint8_t* mem, int copy_size )
{

	uint32_t remain_size;
	/* 
	 * tail > head 
	 * tail is before head */
	if( p_rb->tail > p_rb->head ) {

		remain_size = p_rb->end - p_rb->tail + 1;
		if( copy_size > remain_size){
			memcpy(mem, p_rb->tail, remain_size );
			memcpy(mem + remain_size, p_rb->start,  copy_size - remain_size );
			p_rb->tail = p_rb->start + copy_size - remain_size;
		}else{
			memcpy(mem, p_rb->tail, copy_size);
			p_rb->tail = p_rb->tail + copy_size;

			if(p_rb->tail = p_rb->end + 1){
				p_rb->tail = p_rb->start;
			}

		}

	}else{
		
	/* 
	 * tail <= head
	 * tail is after or equal  head  */
		memcpy(mem, p_rb->tail, copy_size);
		p_rb->tail += copy_size;


	}

	if(p_rb->tail == p_rb->head){
		p_rb->full_flag = BUFFER_EMPTY;
	}

}

int check_data_size()
{
	int size;
	if(p_rb->head > p_rb->tail){
		
		size = p_rb->head - p_rb->tail;

	}else if(p_rb->head < p_rb->tail){
		size = p_rb->end - p_rb->tail + 1 + p_rb->head - p_rb->start;

	}else if( p_rb->full_flag == BUFFER_FULL){
		size = p_rb->size;

	}else{
		size = 0;
	}

	printf("\n%s()>> data_size = %d\n", __FUNCTION__, size);
	return size;

}

int check_available_buffer_size()
{
	int buffer_size;
	buffer_size = p_rb->size - check_data_size();
	return buffer_size;
}

void read_buffer_and_write_to_file( FILE * output_file, int buffer_reading_size )
{
	uint8_t *mem;

	mem = (uint8_t*)malloc(buffer_reading_size);

	/* read_size should be no more than available size */
 	reading_rb( mem,  buffer_reading_size );

	fwrite(mem, 1, buffer_reading_size,  output_file);

	free(mem);

	/* read buffer and write  to file */
// reading_rb(uint8_t* mem, int copy_size )


}


 uint16_t  read_file_and_write_to_buffer(FILE* input_file, int buffer_writing_size )
{

	uint8_t *mem;
	uint16_t ret;

	mem = (uint8_t *)malloc(buffer_writing_size);


	ret = fread(mem, 1, buffer_writing_size,input_file);
//void write_rb( uint8_t *mem, int mem_size)
	write_rb( mem,  buffer_writing_size);

	free(mem);

	return ret;

}

#define FILE_INPUT "file_input.jpg"
#define FILE_OUTPUT "file_output.jpg"
#define MAXIMUM_SIZE  1000
int size_to_write()
{
	int random_size, available_buffer_size, buffer_writing_size, buffer_reading_size; 

	random_size = random()%MAXIMUM_SIZE +1;

	available_buffer_size = check_available_buffer_size();

	if(random_size > available_buffer_size){
		buffer_writing_size = available_buffer_size;

	}else{
		buffer_writing_size = random_size;

	}
	return buffer_writing_size;
}

int size_to_read()
{
	int random_size, available_data_size, buffer_writing_size, buffer_reading_size; 

	random_size = random()%MAXIMUM_SIZE +1;

	available_data_size = check_data_size();

	if(random_size > available_data_size){
		buffer_reading_size = available_data_size;

	}else{
		buffer_reading_size = random_size;

	}
	return buffer_reading_size;
}
int main(int argc, char *argv[])
{
	FILE *file_input, *file_output;
	int random_size, available_buffer_size, buffer_writing_size, buffer_reading_size; 
	int read_size;

	file_input = fopen(FILE_INPUT, "r");
	file_output = fopen(FILE_OUTPUT, "r");

	printf("\ngetting init_rb.\n");
	init_rb( MAXIMUM_SIZE);

	
	printf("\ngetting size_to_write()\n");
	buffer_writing_size = size_to_write();


	read_size = read_file_and_write_to_buffer( file_input,  buffer_writing_size);

	while ( read_size == buffer_writing_size){
		printf("\nloop with buffer_writing_size = %d\n", buffer_writing_size);

		buffer_reading_size = size_to_read();
		printf("\nloop with buffer_reading_size = %d\n", buffer_reading_size);
 		read_buffer_and_write_to_file( file_output,  buffer_reading_size );

		read_size = read_file_and_write_to_buffer( file_input,  buffer_writing_size );

	}
 		read_buffer_and_write_to_file( file_output,  read_size );


}

