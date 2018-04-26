#include<sys/types.h>
#include<sys/wait.h>	//new
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>	//new
int value = 5;
int main(){
	pid_t pid;
	pid = fork();
	if (pid == 0){ // child process
		value += 15;
	}
	else if (pid > 0){ // parent process	
		wait(NULL);
		printf("PARENT:value=%d\n",value); // LINE A
		exit(0);	//new
	}
}
