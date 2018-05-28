#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

//产品
buffer_item buffer[BUFFER_SIZE] = {0};
int head = 0, tail = 0;
sem_t mutex, full, empty;

//保存线程的数据结构
struct Threads
{
	int pid;   // id of thread
	char type; // P or C
	int stime; // sleep time
	int rtime; // run time
	int id;    // the id of production
};

bool Empty()
{
	return head == tail && buffer[head] == 0;
}
bool Full()
{
	if ((tail + 1) % BUFFER_SIZE == head && buffer[tail] != 0)
		return true;
	return false;
}
int insert_item(buffer_item item)
{
	if (!Full())
	{
		buffer[tail] = item;
		tail = (tail + 1) % BUFFER_SIZE;
		return 0;
	}
	return -1;
}

int remove_item(buffer_item *item)
{
	buffer_item t = *item;
	
	if (!Empty())
	{
		if (buffer[t] != 0)
		{
			*item = buffer[t];
			for (; t < tail; t++)
			{
				buffer[t] = buffer[(t + 1) % BUFFER_SIZE];
			}
			buffer[tail] = 0;
			tail = (tail + BUFFER_SIZE - 1) % BUFFER_SIZE;
			
			return 0;
		}
	}
	return -1;
}

void *producer(void *param)
{
	struct Threads* data = (struct Threads*)param;

	while (true)
	{
		sem_wait(&empty);
		sleep(data->stime);
		sem_wait(&mutex);

		printf("Producer %d produced %d\n", data->pid, data->id);
		if (insert_item(data->id) == 0)
			printf("insert %d !\n", data->id);
		else
			printf("insert %d  failed!\n", data->id);

		sleep(data->rtime);
		sem_post(&mutex);
		sem_post(&full);
		pthread_exit(0);
	}
}

void *consumer(void *param)
{
	struct Threads* data = (struct Threads*)param;

	while (true)
	{
		sem_wait(&full);
		sleep(data->stime);
		sem_wait(&mutex);

		buffer_item r;
		srand(time(0));
		r = rand() % (4) + 0;

		printf("Consumer %d wants to consume buffer[%d]\n", data->pid, r);
		if (remove_item(&r) == 0)
			printf("comsume %d !\n", r);
		else
			printf("consume failed !\n");

		sleep(data->rtime);
		sem_post(&mutex);
		sem_post(&empty);
		pthread_exit(0);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Please input the producer number and consumer number!\n");
		exit(0);
	}

	int total = atoi(argv[1]);
	//创建线程数组
	struct Threads info[total];
	//创建线程id数组
	pthread_t ID[total];
	//初始化变量
	sem_init(&mutex, 0 ,1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	//创建数组记录读取文件的数据
	char record[100];

	//读取测试文件
	FILE *file = fopen("t1.txt", "r");
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
		if (info[j].type == 'P')
			info[j].id = record[i++] - '0';
	}

	for (i = 0; i < total; i++)
	{
		//创建生产者线程
		if (info[i].type == 'P')
		{
			printf("Create Producer %d!\n", info[i].pid);
			pthread_create(&ID[i], NULL, producer, &info[i]);
		}
		//创建消费者线程
		if (info[i].type == 'C')
		{
			printf("Create Consumer %d!\n", info[i].pid);
			pthread_create(&ID[i], NULL, consumer, &info[i]);
		}
	}
	
	for (i = 0; i < total; i++)
	{
		pthread_join(ID[i], NULL);
	}
	
	sem_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);

	//输出产品数组
	printf("The buffer array:\n");
	for (int i = 0; i < BUFFER_SIZE; i++)
		printf("%d ", buffer[i]);
	printf("\n");
	exit(0);
}