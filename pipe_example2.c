#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //processes.h
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //to catch children 

/*In this program we will sum the numbers of an array. 2 children processes  will sum one half each, the parent will get their results, and sum them both*/

int main(int argc, char* argv[]){
	int arr[] = {1, 2, 3, 4, 1, 2, 7};
	
	//we need to know the arraysize so as it works with all posible arrays
	int arrSize = sizeof(arr)/sizeof(int);
	int start, end, id1, id2;
	int partialResult = 0;
	
	//we need a file descriptor to read  write from the first pipe
	int fd1[2];
	if(pipe(fd1) == -1){
		printf("There was a mistake creating the first pipe\n");
	}
	
	//we need a second file descriptor to read  write from the second pipe
	int fd2[2];
	if(pipe(fd2) == -1){
		printf("There was a mistake creating the second pipe\n");
	}
	
	//we create the first child process
	id1 = fork();
	if(id1 == -1 ){
		printf("There was a mistake creating the first child\n");
	}
	
	//I set the bounds of the child process
	if(id1 == 0){ // we are inside the first child
		start = 0;
		end = arrSize/2;
		
	} else{ // at the parent process
		//we must create here the second child process
		id2 = fork();
		if(id2 == -1 ){
			printf("There was a mistake creating the second child\n");
		}
		
		//we set the boundaries of the second child process
		if(id2 == 0){
			start = arrSize/2;
			end = arrSize;
		}
		
	}
	
	//we do the sum operation un both processes 
	//if we are within any of the children
	if( id1 == 0 || id2 == 0){
		printf("Entered\n");
		int i;
		for(i = start; i < end ; i++){
			partialResult += arr[i];
		}
		printf("The partial sum of the the child process is %d \n", partialResult);
	}
	
	
	
	//now we will sum both results
	
	if(id1 == 0){//Im at the first child process, I have to send my result to my parent
		//Im not reading anything
		close(fd1[0]);
		//write (int fd, void* buf, size_t cnt)
		//Writes cnt bytes from buf to the file or socket associated with fd. cnt 
		if(write(fd1[1], &partialResult, sizeof(int)) == -1){
			printf("An error has ocurred while writing in childs fd[1]\n");
		}
		//close the pipe Im not using
		close(fd1[1]);
	} else if (id2 == 0){
		//Im not reading anything
		close(fd2[0]);
		//write (int fd, void* buf, size_t cnt)
		//Writes cnt bytes from buf to the file or socket associated with fd. cnt 
		if(write(fd2[1], &partialResult, sizeof(int)) == -1){
			printf("An error has ocurred while writing in childs fd[1]\n");
		}
		//close the pipe Im not using
		close(fd2[1]);
	} else{ // I'm at the parent process
		
		int sumChild1, sumChild2;
		int totalSum = 0;
		//Im not writing
		close(fd1[1]);
		close(fd2[1]);
		if(read(fd1[0], &sumChild1, sizeof(int)) == -1){
				printf("There was a error while reading from fd1\n");
		}
		close(fd1[0]);
		if(read(fd2[0], &sumChild2, sizeof(int)) == -1){
				printf("There was a error while reading from fd2\n");
		}
		close(fd2[0]);
		totalSum = sumChild1 + sumChild2;
		printf("Total sum is %d\n", totalSum);
		
		//I wait for children to finish
		while(wait(NULL) != -1 || errno != ECHILD){
				printf("Waited for a child to finish\n");
		}
		
	}
	
	return 0;
		
}
