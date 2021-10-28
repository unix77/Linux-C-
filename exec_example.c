#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //work with procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //to catch children processes 

/*This program shows how execl can be used with post processing, within a process tree*/


int main(int argc, char* argv[]){
	
	int id = fork();
	if(id == -1){
		printf("An error has ocurred creating a child process");
	}
	
	if(id == 0){//child process code
		//execv(path_to_binary, path_to_binary,params_for_new_program, NULL);
		execl("/bin/ls", "/bin/ls", NULL);
	} else{//parent process
		while(wait(NULL) != -1 || errno != ECHILD){
			printf("Waited for a child to finish\n");
		}
		printf("Success !\n");
		printf("Post processing has been executed !\n");
	}
	
	
	return 0;
		
}
