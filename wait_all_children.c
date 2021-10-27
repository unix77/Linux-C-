#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para trabajar con procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //para usar en el bucle capturador de procesos hijo




int main(int argc, char* argv[]){
	int id = fork();
	int n;
	if(id == 0){ // child process
		n = 1;
	} else {	// parent process
		n = 6;
		//it waits for any child process to finish
		//this will let us print the first 5 numbers by the child
		//and then print from 6 to 10, the other numbers
		while(wait(NULL) != -1 || errno != ECHILD){
				printf("Waited for a child to finish\n");
		}
	}
	
	int i;
	for(i = n ; i < n+5 ; i++){
			printf("%d \n", i);
			fflush(stdout);//it shows what we want, immediately on the screen
	}
	return 0;
		
}
