#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<time.h>

#define MAXRAND 100
int data = 0;
//读者数量及写者数量
int read_count = 0, write_count = 0;
//writeM是对write_count的互斥，readM是对read_count的互斥
//mutexR对READ进行互斥，  mutexW当写者在写或者等待时，阻塞读者
sem_t writeM, readM, mutexR, mutexW;

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
		sem_wait(&writeM);

		write_count++;
		if (write_count == 1)
			sem_wait(&mutexR);
		sem_post(&writeM);

		sem_wait(&mutexW);
		printf("Writer %d is Writting\n", th->pid);
		WRITE();

		sleep(th->rtime);
		printf("writer %d stopped\n", th->pid);
		sem_post(&mutexW);

		sem_wait(&writeM);
		write_count--;
		if (write_count == 0)
			sem_post(&mutexR);
		sem_post(&writeM);

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
		sem_wait(&mutexR);
		sem_wait(&readM);

		read_count++;
		if (read_count == 1)
			sem_wait(&mutexW);
		sem_post(&readM);
		sem_post(&mutexR);

		printf("Reader %d is reading\n", th->pid);
		READ();

		sleep(th->rtime);
		printf("Reader %d stopped\n", th->pid);
		
		sem_wait(&readM);
		read_count--;
		if (read_count == 0)
			sem_post(&mutexW);
		sem_post(&readM);

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
	sem_init(&writeM, 0 ,1);
	sem_init(&readM, 0, 1);
	sem_init(&mutexR, 0 ,1);
	sem_init(&mutexW, 0, 1);

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

	sem_destroy(&writeM);
	sem_destroy(&readM);
	sem_destroy(&mutexR);
	sem_destroy(&mutexW);

	exit(0);
}
