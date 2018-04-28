#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#define MAX_LINE 80
#define BUFFER_SIZE 50
char buffer[BUFFER_SIZE];

void handle_SIGINT()
{
	write(STDOUT_FILENO, buffer, strlen(buffer));
	exit(0);
}

void setup(char inputBuffer[], char *args[], int *background)
{
	int length, i, start, ct; 
	ct = 0;
	
	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
	start = -1;
	
	if (length == 0)
		exit(0);
	if (length < 0)
	{
		perror("error reading the command!\n");
		exit(-1);
	}
	for (i = 0; i< length; i++)
	{
		switch(inputBuffer[i])
		{
			case ' ':
			case '\t':
				if (start != -1)
				{
					args[ct] = &inputBuffer[start];
					ct++;
				}
				inputBuffer[i] = '\0';
				start = -1;
			break;
			case '\n':
				if (start != -1)
				{
					args[ct] = &inputBuffer[start];
					ct++;
				}
				inputBuffer[i] = '\0';
				args[ct] = NULL;
			break;
			default :
				if (start == -1)
					start = i;
				if (inputBuffer[i] == '&')
				{
					*background = 1;
					inputBuffer[i] = '\0';
				}
		}	
	}
	args[ct] = NULL;
} 

int main(void)
{
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	sigaction(SIGINT, &handler, NULL);

	strcpy(buffer, "Caught Control+C!\n");

	char inputBuffer[MAX_LINE];
	int background;
	char *args[MAX_LINE/2 + 1];
	
	while (1)
	{
		background = 0;
		printf(" CAMMAND->");
		fflush(stdout);
		setup(inputBuffer, args, &background);
		pid_t pid ;
		pid = fork();
		if (pid == 0)
		{
			execvp(args[0], args);
		}
		else
		{
			if (!background)
				waitpid(pid, NULL, 0);
		}
		
	}
}
