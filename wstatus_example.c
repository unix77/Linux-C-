#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //work with procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //to catch children processes 

/*This program shows how we can catch a process termination status within its parent*/


int main(int argc, char* argv[]){
	
	int id = fork();
	if(id == -1){
		printf("An error has ocurred creating a child process");
	}
	
	if(id == 0){//child process code
		//execv(path_to_binary, path_to_binary,params_for_new_program, NULL);
		execlp("ping", "ping", "-c", "1", "google.con", NULL);
	} else{//parent process
		int wstatus;
		wait(&wstatus);
		printf("wstatus : %d\n",wstatus);
		//WIFEXITED is true if the wstatus was successful
		if (WIFEXITED(wstatus)){
			//any other exit status value != 0 means a failure 
			if(WEXITSTATUS(wstatus) == 0 ){
				printf("Success !\n");
			} else{
				printf("Failure! with status code %d !\n", wstatus);
			}
		}
		printf("Post processing has been executed !\n");
	}
	
	
	return 0;
		
}
