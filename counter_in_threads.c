
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>

volatile int counter = 0;
//int counter = 0;
//sem_t sem_info_thread;
pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * pthread_func(void * arg){

			pthread_mutex_lock(&mutext);
	while(1){

		do{

				printf("thread2: counter = %d\n",counter);
			while(counter ==8){
			//if(counter ==8){
				printf("thread2: wait start\n");
				pthread_cond_wait(&cond,&mutext);
				printf("thread2: wait end\n");
			}
			counter++;

		}while(counter<20);
		counter = 20;
		}

}







int main(int argc, char *argv[]){

	pthread_t a_thread,b_thread;
	pthread_create(&a_thread,NULL, pthread_func, NULL);
	int i;

	while(1){

			printf("main_thread: counter = %d\n",counter);
			pthread_mutex_lock(&mutext);
		if(counter ==8){
			pthread_cond_signal(&cond);
			counter = 0;
			printf("main_thread: counter changed = %d\n",counter);
		}
			pthread_mutex_unlock(&mutext);
		//sched_yield();
		//usleep(0);
		/*
		for(i=0;i<10000;i++)
			printf(".",i);
			*/
	}

	pthread_join (a_thread,NULL);
	exit(0);
	


}
