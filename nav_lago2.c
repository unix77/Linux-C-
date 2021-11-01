#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/wait.h>
#include <errno.h> 
#include <pthread.h>
#include <semaphore.h>

#define PASAJEROS 100 //son todas las personas que quieren comprar un ticket (para cq categoria)

// first class thread
//   wait sem_first_class_line

// business thread
//   wait sem_business_line
//   post sem_go_to_departure_line

// tourist thread
//   wait sem_tourist_line

// passenger thread
//   select a ticket to buy
//   post sem_first_class_line || sem_business_line || sem_tourist_line
//   wait sem_go_to_departure_line

/*--------------------------------------------------------------------------*/

//tenemos que crear N threads que van a competir por comprar un ticket (primera || business || turista )


/*--------------------------------------------------------------------------*/

/* FUNCIONES */
/* pasajero */

//   select a ticket to buy
//   post sem_first_class_line || sem_business_line || sem_tourist_line
//   wait sem_go_to_departure_line

/* -------------------------------------------------------------SEMAFOROS---------------------------------------------------------------------------- */

sem_t cprimera, cbusiness, cturista, comprar, embarcar, desembarcar;

/* -------------------------------------------------------------FUNCIONES---------------------------------------------------------------------------- */
/*Van ciclando entre ellos, se tira*/

int fprimera();
int fbusiness();
int fturista();

int fprimera(){//tienen while(1)
	if(sem_trywait(&cprimera) == 0){
		//sem_post(&embarcar);
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fbusiness();
	}	
	return 0;
}
				
int fbusiness(){//tienen while(1)
	if(sem_trywait(&cbusiness) == 0){
		//sem_post(&embarcar);
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fturista();
	}	
	return 0;
}

int fturista(){//tienen while(1)
	if(sem_trywait(&cturista) == 0){
		//sem_post(&embarcar);
		sem_post(&comprar);
	}else{
		//si ya se vendieron todos los pasajes de la primera clase, mando para que compre un ticket de la clase business
		fprimera();
	}	
	return 0;
}

/* pasajero */
void* fpasajero(){
	int r;
	time_t t;

	//bloqueo el semaforo para poder comprar un ticket
	sem_wait(&comprar);
	srand((unsigned) time(&t));
	usleep(50000);
	r =  rand() % 3;
	
	if(r == 0){
		fprimera();	
	} else if (r == 1){
		fbusiness();
	} else{
		fturista();	
	}/*
	switch(r){
		case(0):
			sem_post(&atenderprimera);
			break;
		case(1):
			sem_post(&atenderbusiness);
			break;
		case(2):
			sem_post(&atenderturista);
			break;*/

	//el pasajero ya tiene el ticket, va a embarcar
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
	//espero que los pasajeros bajen el valor de embarcar a 0
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
	//semaforos contadores de tickets disponibles 
	sem_init(&cprimera, 0, 20);
	sem_init(&cbusiness, 0, 30);
	sem_init(&cturista, 0, 50);
	//semaforo contador de cantidad 
	//de pasajeros que ya embarcaron
	sem_init(&embarcar, 0, 0);
	//semaforo binario de pasajero 
	//comprando un ticket
	sem_init(&comprar, 0, 1);
	//semaforo contador de pasajeros que ya desembarcaron
	sem_init(&desembarcar, 0, 0);
	
	printf("Semaforos inicializados \n");
	
	/*Inicializacion de los hilos*/
	pthread_t pasajero[PASAJEROS], barco; //cajeroPrimera, cajeroBusiness, cajeroTurista;
	/*
	int svcp, svcb, svct, sve, svz, svc, svd, svspas, atp, atb, att;
	
	
	//obtenego el valor de todos los semaforos
	sem_getvalue(&cprimera, &svcp);
	sem_getvalue(&cbusiness, &svcb);
	sem_getvalue(&cturista, &svct);
	sem_getvalue(&embarcar, &sve);
	sem_getvalue(&zarpar, &svz);
	sem_getvalue(&comprar, &svc);
	sem_getvalue(&desembarcar, &svd);
	sem_getvalue(&spasajeros, &svspas);
	sem_getvalue(&atenderprimera, &atp);
	sem_getvalue(&atenderbusiness, &atb);
	sem_getvalue(&atenderturista, &att);
	
	printf("PASAJEROS EN PRIMERA %d \n", svcp);
	printf("PASAJEROS EN BUSINESS %d \n", svcb);
	printf("PASAJEROS EN TURISTA %d \n", svct);
	printf("EN EMBARCAR %d \n", sve);
	printf("EN ZARPAR %d \n", svz);
	printf("EN COMPRAR  %d \n", svc);
	printf("EN DESEMBARCAR %d \n", svd);
	printf("PASAJEROS TOTAL %d \n", svspas);
	printf("ATENDER PRIMERA %d \n", atp);
	printf("ATENDER BUSINESS %d \n", atb);
	printf("ATENDER TURISTA %d \n", att);
	*/
	
	int i;
	for (i = 0 ; i < PASAJEROS ; i++){
		if(pthread_create(&pasajero[i], NULL, &fpasajero, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
		}
	}
	
	if(pthread_create(&barco, NULL, &fbarco, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
	}
	/*
	if(pthread_create(&cajeroPrimera, NULL, &fprimera, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
	}
	
	if(pthread_create(&cajeroBusiness, NULL, &fbusiness, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
	}
	
	if(pthread_create(&cajeroTurista, NULL, &fturista, NULL) != 0){
			printf("Ocurrio un error al crear el hilo\n");
	}
	*/
	printf("Hilos inicializados \n");
	
	/*
	//obtenego el valor de la cantidad de pasajeros que van a viajar
	sem_getvalue(&embarque, &pas);
	sem_getvalue(&cp, &pri);
	sem_getvalue(&cb, &bus);
	sem_getvalue(&ct, &tur);
	sem_getvalue(&pretendientes, &pret);
	sem_getvalue(&viaje, &via);
	*/
	
	for (i = 0 ; i < PASAJEROS ; i++){
		if(pthread_join(pasajero[i], NULL) != 0){
			printf("An error ocurred joining the thread\n");
		}
		//printf("Thread %d has ended \n", i);
	}
	
	if(pthread_join(barco, NULL) != 0){
		printf("An error ocurred joining the thread\n");
	}
	
	return 0;
}
