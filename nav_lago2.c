#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/wait.h>
#include <errno.h> 
#include <pthread.h>
#include <semaphore.h>

#define PASAJEROS 100 

/* -------------------------------------------------------------SEMAFOROS---------------------------------------------------------------------------- */

sem_t cprimera, cbusiness, cturista, comprar, embarcar, desembarcar;

/* -------------------------------------------------------------FUNCIONES---------------------------------------------------------------------------- */

/*Declaracion de funciones*/
void fprimera();
void fbusiness();
void fturista();

/*Vende un ticket de la primera clase */
void fprimera(){
	//intenta vender un ticket de la primera clase 
	if(sem_trywait(&cprimera) == 0){
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fbusiness();
	}	
}
	
/*Vende un ticket de la business clase */				
void fbusiness(){
	if(sem_trywait(&cbusiness) == 0){
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fturista();
	}	
}

/*Vende un ticket de la clase turista */
void fturista(){
	if(sem_trywait(&cturista) == 0){
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fprimera();
	}	
}

/* pasajero */
void* fpasajero(){
	int r;
	time_t t;

	//bloqueo el semaforo para poder comprar un ticket
	sem_wait(&comprar);
	srand((unsigned) time(&t));
	usleep(50000);
	r =  rand() % 3 + 1;
	
	if(r == 1){
		fprimera();	
	} else if (r == 2){
		fbusiness();
	} else{
		fturista();	
	}
	
	//el pasajero ya compro el ticket, va a embarcar
	sem_post(&embarcar);
	printf("Embarco un pasajero \n");
	//ya llegamos a destino, se puede desembarcar
	sem_wait(&desembarcar);
	
	usleep(50000);
	//sem_post(&desembarcar);
	printf("Desembarco un pasajero \n");
	return 0;
}

/*barco*/
void* fbarco(){
	int j;
	for(j = 0 ; j < PASAJEROS; j++){
		sem_wait(&embarcar);
	}
	printf("EL barco comenzo su viaje\n");
	
	for(j = 0 ; j < PASAJEROS; j++){
		sem_post(&desembarcar);
	}
	return 0;
}

int main(int argc, char* argv[]){
	/*Inicializacion de los semaforos*/
	//contadores de tickets disponibles 
	sem_init(&cprimera, 0, 20);
	sem_init(&cbusiness, 0, 30);
	sem_init(&cturista, 0, 50);
	//permiso a embarcar
	sem_init(&embarcar, 0, 0);
	//permiso a comprar un ticket
	sem_init(&comprar, 0, 1);
	//semaforo contador de pasajeros que ya desembarcaron
	sem_init(&desembarcar, 0, 0);
	
	printf("Semaforos inicializados \n");
	
	/*Inicializacion de los hilos*/
	pthread_t pasajero[PASAJEROS], barco; //cajeroPrimera, cajeroBusiness, cajeroTurista;
	
	int i;
	for (i = 0 ; i < PASAJEROS ; i++){
		if(pthread_create(&pasajero[i], NULL, &fpasajero, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
		}
	}
	
	if(pthread_create(&barco, NULL, &fbarco, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
	}
	
	printf("Hilos inicializados \n");
	
	for (i = 0 ; i < PASAJEROS ; i++){
		if(pthread_join(pasajero[i], NULL) != 0){
			printf("An error ocurred joining the thread\n");
		}
	}
	
	if(pthread_join(barco, NULL) != 0){
		printf("An error ocurred joining the thread\n");
	}
	
	return 0;
}
