#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para trabajar con procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //para usar en el bucle capturador de procesos hijo
#include <pthread.h>

#define THN  4

int a = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* operation(){
	pthread_mutex_lock(&mutex);
	a++; //crit section
	pthread_mutex_unlock(&mutex);
	
	return 0;
}

int main(int argc, char* argv[]){
	pthread_t th[THN];
	int i;
	for (i = 0 ; i < THN ; i++){
		if(pthread_create(&th[i], NULL, &operation, NULL) != 0){
			printf("An error ocurrec creating the thread\n");
		}
		printf("Thread %d has started \n", i);
	}
	
	for (i = 0 ; i < THN ; i++){
		if(pthread_join(th[i], NULL) != 0){
			printf("An error ocurrec creating the thread\n");
		}
		printf("Thread %d has ended \n", i);
	}
	
	printf("The shared variable value is %d \n", a);
	
	return 0;
}
