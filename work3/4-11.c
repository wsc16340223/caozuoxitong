#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#define MAX 10
int fibo[MAX]; //线程共享的数组
void *runner(void *param);

int main(int argc, char *argv[])
{
    pthread_t tid; //线程id
    pthread_attr_t attr; //线程的属性

    if (argc != 2)
    {
        fprintf(stderr, "Usage: a.out <integer value>\n");
        return -1;
    }
    if (atoi(argv[1]) < 0 || atoi(argv[1]) > 10)
    {
        fprintf(stderr, "Integer value must >=0 and <= 10\n");
        return -1;
    }

    
    pthread_attr_init(&attr); // 初始化属性为默认属性
    pthread_create(&tid, &attr, runner, argv[1]); //创建线程
    pthread_join(tid, NULL); // 阻塞调用线程，直到tid所指定的线程终止
    
    
    int i, upper = atoi(argv[1]); // 获取输入的参数
    for (i = 0; i < upper; i++) //输出fibo
    {
        printf("fibo %d:\t%d\n", i, fibo[i]);
    }
    
}

void *runner(void *param)
{
    int i, upper = atoi(param); //获取输入的参数
    fibo[0] = 0; //初始化fibo
    fibo[1] = 1;
    for (i = 2; i < upper; i++) //给fibo赋值
    {
        fibo[i] = fibo[i-1] + fibo[i-2];
    }
    pthread_exit(0); //终止调用线程
}