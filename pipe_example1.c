#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //para trabajar con procesos
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //para usar en el bucle capturador de procesos hijo

/*In this program we will sum the numbers of an array
 * The child will sum the first n/2 elements, the paren the rest of them, and sum both 
 * results afterwards.*/

int main(int argc, char* argv[]){
	int arr[] = {1, 2, 3, 4, 1, 2, 7};
	
	//we need to know the arraysize so as it works with all posible arrays
	int arrSize = sizeof(arr)/sizeof(int);
	int start, end;
	int partialResult = 0;
	
	//we need a file descriptor to read  write
	int fd[2];
	if(pipe(fd) == -1){
		printf("There was a mistake creating the pipe\n");
	}
	
	//we create a child process
	int id = fork();
	if(id == -1 ){
		printf("There was a mistake creating the child\n");
	}
	
	//I set the bounds of each process
	if(id == 0){ // we are inside the child
		start = 0;
		end = arrSize/2;
		
	} else{ // at the parent process
		start = arrSize/2;
		end = arrSize;
	}
	
	int i;
	for(i = start; i < end ; i++){
		partialResult += arr[i];
	}
	
	printf("The partial sum of the process is %d \n", partialResult);
	
	//now we will sum both results
	
	if(id == 0){//Im at the child process, I have to send my result to my parent
		//Im not reading anything
		close(fd[0]);
		//write (int fd, void* buf, size_t cnt)
		//Writes cnt bytes from buf to the file or socket associated with fd. cnt 
		if(write(fd[1], &partialResult, sizeof(int)) == -1){
			printf("An error has ocurred while writing in childs fd[1]\n");
		}
		//close the pipe Im not using
		close(fd[1]);
	} else{//parent process
		int sumChild = 0;
		int totalSum = 0;
		//Im not writing
		close(fd[1]);
		if(read(fd[0], &sumChild, sizeof(int)) == -1){
				printf("There was a error while reading from fd\n");
		}
		close(fd[0]);
		totalSum = partialResult + sumChild;
		printf("Total sum is %d\n", totalSum);
		
		//I wait for children to finish
		while(wait(NULL) != -1 || errno != ECHILD){
				printf("Waited for a child to finish\n");
		}
		
	}
	
	return 0;
		
}
