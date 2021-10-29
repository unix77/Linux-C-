#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //processes.h
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //to catch children 

/*In this program we will generate a set of numbers in a child process,
 * send them to the parent process
 * the parent will return the sum of all of them*/

int main(int argc, char* argv[]){
	
	
	int size, i;
	int fd[2];
	if(pipe(fd) == -1){
		printf("There was a mistake creating the pipe\n");
	}
	
	int id = fork();
	if(id == -1){
		printf("There was a mistake creating a child\n");
	}
	
	if(id == 0){//at the child process
		//close the reading fd
		close(fd[0]);
		srand(time(NULL));
		size = rand() % 10 + 1;
		int arr[size];
		for( i = 0 ; i < size ; i++){
			arr[i] = rand() % 11;
			printf("Arr %d = %d \n",i, arr[i]);
		}
		//I write the size of the array "n"
		if(write(fd[1], &size, sizeof(size)) == -1){
			printf("An error has ocurred while writing in childs fd[1]\n");
		}
		//I write the array at the fd[1]
		if(write(fd[1], arr, sizeof(arr) * size) == -1){
			printf("An error has ocurred while writing in childs fd[1]\n");
		}
		//I close the writing fd
		close(fd[1]);
	}else {//parent process
		int result = 0;
		//I wont write anything
		close(fd[1]);
		if(read(fd[0], &size, sizeof(int)) == -1){
				printf("There was a error while reading from fd1\n");
		}
		printf("The array size at the parent is = %d \n", size);
		int arrparent[size];
		if(read(fd[0], arrparent, sizeof(int) * size) == -1){
				printf("There was a error while reading from fd1\n");
		}
		close(fd[0]);
		
		for( i = 0 ; i < size ; i++){
			result += arrparent[i];
			printf("ParentArr %d = %d \n",i, arrparent[i]);
		}
		printf("Sum result is %d \n", result);
		
		while(wait(NULL) != -1 || errno != ECHILD){
				printf("Waited for a child to finish\n");
		}
	}
	
	
	return 0;
		
}
