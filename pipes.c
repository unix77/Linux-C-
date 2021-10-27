#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para trabajar con procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //para usar en el bucle capturador de procesos hijo

int main(int argc, char* argv[]){
	int fd[2];
	//fd[0] - read fd
	//fd[1] - write fd
	
	//abrimos un pipe, e iniciamos los file descriptors (1 para leer y el otro para escribir)
	//si hacemos fork despues de esto, el hijo va a "heredar" los fd, pero no son los mismos que los de su padre !
	//si el padre cierra un fd, el hijo no lo ve ! 
	if(pipe(fd) == -1){
		printf("Un error sucedio al abrir el pipe");
		return 1;	
	}
	
	int id = fork();
	if(id == -1){
		printf("An error happened during the fork operation\n");
	}
	if(id == 0){ //estoy en el hijo
		//we aren't going to read anything at the child proc, so we close the fd[0]
		close(fd[0]);
		int x;
		printf("Insert a number: ");
		scanf("%d", &x);
		//we write the X inside of the write file descriptor of the child process
		//write (int fd, void* buf, size_t cnt)
		//Writes cnt bytes from buf to the file or socket associated with fd. cnt 
		if(write(fd[1], &x, sizeof(int)) == -1){
			printf("An error has ocurred while writing\n");
		}
		//after you have written, you close the file descriptor
		close(fd[1]);
	} else{ //parent process
		//we won't write anything at the parent process so we can close its writing fd
		close(fd[1]);
		int y;
		//read (int fd, void* buf, size_t cnt)
		//reads cnt bytes from fd[0] and stores it in buf
		if(read(fd[0], &y, sizeof(int)) == 1){
			printf("An error has ocurred while reading\n");
		}
		//we won't read anymore so we can close the reading fd
		close(fd[0]);
		//the value we read from its child is 
		printf("The integer value we read from file is %d \n", y);
		
		//shouldn't we use a wait here ?? 
		//is a wait always necessary?
		while(wait(NULL) != -1 || errno != ECHILD){
				printf("Waited for a child to finish\n");
		}
			
	}
	
	
	return 0;
		
}
