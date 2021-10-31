#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para trabajar con procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //para usar en el bucle capturador de procesos hijo

void* prOne(){
	printf("0 \n");
	return 0;
	
}

void* prTwo(){
	printf("2 \n");	
return 0;
}

void* prThree(){
	printf("3 \n");
	return 0;
}

int main(int argc, char* argv[]){
	int id[6];
	//fd[0] - read fd
	//fd[1] - write fd
	
	for(int j = 0 ; j < 3 ; j++){
		id[j] = fork();
		if(id[j] == -1){
			printf("Error \n");
		}
		if (id[j] == 0){ // child
			printf("Aloha, I'm %d\n",id[j]);
			if(j == 0){
				prOne();
			} else if (j == 1){
				prTwo();
			} else {
				prThree();
			}
			exit(id[j]);
		}else{ // parent
			printf("Ohana \n");
		}
		
	}
	while(wait(NULL) != -1 || errno != ECHILD){
			printf("Waited for a child to finish\n");
	}
	return 0;
		
}
