#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<time.h>

#define MAXRAND 100
int data = 0;

int read_count = 0;

sem_t writer, mutex;

struct Threads
{
	int pid;	// id of thread
	char type;	// W or R
	int stime;	// sleep time
	int rtime;	// run time
};

void WRITE()
{
	int rd = rand() % MAXRAND;
	printf("Write %d\n", rd);
	data = rd;
}

void READ()
{
	printf("Read %d\n", data);
}

void *Writer(void *param)
{
	struct Threads* th = (struct Threads*)param;

	while(true)
	{
		sleep(th->stime);
		printf("Writer %d wants to write\n", th->pid);
		sem_wait(&writer);

		printf("Writer %d is Writting\n", th->pid);
		WRITE();

		sleep(th->rtime);
		printf("writer %d stopped\n", th->pid);
		sem_post(&writer);

		pthread_exit(0);
	}
}

void *Reader(void *param)
{
	struct Threads* th = (struct Threads*)param;

	while(true)
	{
		sleep(th->stime);
		printf("Reader %d wants to read\n", th->pid);
		sem_wait(&mutex);

		read_count++;
		if (read_count == 1)
			sem_wait(&writer);
		sem_post(&mutex);

		printf("Reader %d is reading\n", th->pid);
		READ();

		sleep(th->rtime);
		printf("Reader %d stopped\n", th->pid);
		sem_wait(&mutex);

		read_count--;
		if (read_count == 0)
			sem_post(&writer);
		sem_post(&mutex);

		pthread_exit(0);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Please input the Writer number and Reader number!\n");
		exit(0);
	}

	int total = atoi(argv[1]);
	sem_init(&writer, 0 ,1);
	sem_init(&mutex, 0, 1);

	struct Threads info[total];
	pthread_t ID[total];

	char record[100];

	//读取测试文件
	FILE *file = fopen("t2.txt", "r");
	if (file == NULL)
	{
		printf("open file error!\n");
		exit(0);
	}

	char ch;
	int i = 0;
	while(!feof(file))
	{
		ch = fgetc(file);
		if (ch != ' ' && ch != '\n')
			record[i++] = ch;
	}
	i = 0;
	for (int j = 0;j < total;j++)
	{
		info[j].pid = record[i++] - '0';
		info[j].type = record[i++];
		info[j].stime = record[i++] - '0';
		info[j].rtime = record[i++] - '0';
	}

	for (i = 0; i < total; i++)
	{

		if (info[i].type == 'R')
		{
			printf("Create Reader %d\n", info[i].pid);
			pthread_create(&ID[i], NULL, Reader, &info[i]);
		}

		if (info[i].type == 'W')
		{
			printf("Create Writer %d\n", info[i].pid);
			pthread_create(&ID[i], NULL, Writer, &info[i]);
		}
	}

	for (i = 0; i < total; i++)
	{
		pthread_join(ID[i], NULL);
	}

	sem_destroy(&writer);
	sem_destroy(&mutex);

	exit(0);
}
