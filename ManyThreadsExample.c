#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/*What I want the thread to do when the thread is created*/
/* arg: a void pointer*/
void *thread_routine(void *arg)
{
	printf("The thread has started\n");
	int lines = *((int*)arg);/*We use the int value of the object pointed by arg*/
	int fd; /*The file descriptor*/
	char buf[] = "New line\n";
	
	for(int i = 0; i < lines; i++)
	{
		/*we open the txt in writing mode (WRONLY), and each time it writes, we want to do it on a new line*/
		fd = open("/home/pi/git-workspace/Linux-C-/manyThreads.txt",O_WRONLY|O_APPEND);
		/*we write in the file arg:(file descriptor, buffer,number of bytes too be written~ the buffer size -1 because it ends in 0)*/
		write(fd, buf, sizeof(buf)-1);
		close(fd);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	pthread_t thread1; /*Name of the thread we will create*/
	/*atoi: converts string to integer*/
	int value = atoi(argv[1]);/*Number of lines we want to write in the file, it will be read by console*/
	
	/*Creating the thread*/
	/*"pthread_create" returns "0" if it's been successful*/
	/*arg:pointer to thr name, attr~NULL: default att, routine to execute, entry arg to the rounte */
	if(0 != pthread_create(&thread1, NULL, thread_routine, &value))
	{
		return -1;
	}
	
	/*We want Main to wait until the thred will finish*/
	/*args:(threadName, returnValueOfOthread_join)*/
	pthread_join(thread1, NULL);
	
	return 0;
	
}

