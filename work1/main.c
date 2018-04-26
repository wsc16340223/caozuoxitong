#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>


int main(){
	int pid1 = fork();
	printf("**1**\n");
	int pid2 = fork();
	printf("**2**\n");
	if (pid1 == 0){
		int pid3 = fork();
		printf("**3**\n");
		printf("%d\t%d\t%d\t%d\n",pid1,pid2,pid3,getpid());
	}
	else{
		printf("**4**\n");
		printf("%d\t%d\t%d\n",pid1,pid2,getpid());
	}
	return 0;
}
