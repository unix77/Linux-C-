#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/wait.h>
#include <errno.h> 
#include <pthread.h>
#include <semaphore.h>

#define PASAJEROSXTANDA 10
#define PASAJEROSBARCO 100
#define PASAJEROS 500

/* -------------------------------------------------------------SEMAFOROS---------------------------------------------------------------------------- */

sem_t cprimera, cbusiness, cturista, comprar, embarcar;

/* -------------------------------------------------------------FUNCIONES---------------------------------------------------------------------------- */

/*Declaracion de funciones*/
void fprimera();
void fbusiness();
void fturista();

/*Vende un ticket de la primera clase */
void fprimera(){
	sem_post(&comprar);
	sem_wait(&cprimera);
	printf("El pasajero embarco en PRIMERA clase\n");
}
	
/*Vende un ticket de la business clase */				
void fbusiness(){
	sem_post(&comprar);
	sem_wait(&cbusiness);
	printf("El pasajero embarco en BUSINESS clase\n");
}

/*Vende un ticket de la clase turista */
void fturista(){
	sem_post(&comprar);
	sem_wait(&cturista);
	printf("El pasajero embarco en la clase TURISTA\n");
}

/* pasajero */
void* fpasajero(){
	int r;

	//bloqueo del semaforo para que se pueda comprar un ticket
	sem_wait(&comprar);
	srand(time(0));
	r =  rand() % 3 + 1;
	
	//de manera aleatoria se va a comprar el pasaje de cierta clase
	if(r == 1){
		fprimera();	
	} else if (r == 2){
		fbusiness();
	} else{
		fturista();	
	}
	
	//el pasajero ya compro el ticket de una clase, va a embarcar
	sem_post(&embarcar);
	
	//usleep(50000);
	return 0;
}

/*barco*/
void* fbarco(){
	//el barco va a viajar cada vez que se llene el cupo de 100 personas del barco
	while(1){
		int j;
		
		for(j = 0 ; j < PASAJEROSBARCO; j++){
			sem_wait(&embarcar);
		}
		
		//bloqueo de comprar una vez que embarcaron 100, para que no puedan comprar mas pasajes los pasajeros que quedaron pendientes
		sem_wait(&comprar);
		
		printf("-----------------------------------------------------BARCO VIAJANDO--------------------------------------------------------------------------\n");
		//desembarcan todos los pasajeros del barco
		//el barco llego a destino, por cada pasajero de cierta clase que desembarca, puede subir otro que queria comprar un pasaje de est misma clase
		for(j = 0 ; j < 20 ; j ++){
				sem_post(&cprimera);
				printf("Se desocupo un lugar en PRIMERA\n");
		}
		for(j = 0 ; j < 30 ; j ++){
				sem_post(&cbusiness);
				printf("Se desocupo un lugar en BUSINESS\n");
		}
		for(j = 0 ; j < 50 ; j ++){
				sem_post(&cturista);
				printf("Se desocupo un lugar en TURISTA\n");
		}
		
		sem_post(&comprar);
		
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
	
	printf("Semaforos inicializados \n");
	
	/*Inicializacion de los hilos*/
	pthread_t pasajero[PASAJEROS], barco;
	
	if(pthread_create(&barco, NULL, &fbarco, NULL) != 0){
		printf("Ocurrio un error al crear el hilo\n");
	} 
	
	int i = 0;
	int nhilo = 0;
	
	printf("Se crean los hilos pasajeros\n");
	
	/*El programa crea hilos de pasajeros agrupados en tanda cada cierto periodo de tiempo*/
	
	while(nhilo < PASAJEROS){
		for(i = 0 ; i < PASAJEROSXTANDA ; i ++){
			if(pthread_create(&pasajero[nhilo], NULL, &fpasajero, NULL) != 0){
				printf("Ocurrio un error al crear el hilo\n");
				
			}
			nhilo++;
		}
		sleep(2);
	}
	
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
