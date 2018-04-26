#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
	pid_t p1, p2;
	printf("a\n");
	printf("p1\tp2\tpid\n");
	printf("%d\t%d\t%d\n",p1,p2,getpid());
	
	if (p1=fork() == 0) {
		printf("b\n");
		printf("p1\tp2\tpid\n");
		printf("%d\t%d\t%d\n",p1,p2,getpid());
		if (p2=fork() == 0){
			printf("c\n");
			printf("p1\tp2\tpid\n");
			printf("%d\t%d\t%d\n",p1,p2,getpid());
		}
	}
}
