#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
pthread_t workers[10][10]; 

int M, N, K;

int A[10][10];
int B[10][10];
int C[10][10];
//使用结构来传递多个参数
struct v
{
    int i;
    int j;
};
//按照ppt所给的提示 传递i和j之后 按照公式计算
void *runner(void *d)
{
    struct v *data = (struct v*)d;
    int a;
    for (a = 0; a < K; a++)
    {
        C[data->i][data->j] += A[data->i][a] * B[a][data->j];
    }
    pthread_exit(0);
}

int main()
{
    

    int i, j, k = 0;

    printf("Input the row and col of Matrix A:\n");
    scanf("%d%d", &M, &K);
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < K; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Input the row and col of Matrix B:\n");
    scanf("%d%d", &K, &N);
    for (i = 0; i < K; i++)
    {
        for (j = 0; j < N; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            //struct v *data = (struct v *)malloc(sizeof(struct v));
            struct v data;
            data.i = i;
            data.j = j;
            //创建线程 待其终止后阻塞它
            pthread_create(&workers[i][j], NULL, (void *)runner, (void *)&data);
            pthread_join(workers[i][j], NULL);
            k++;
        }
    }
/*
    for (i = 0; i < NUM_THREADS - 1; i++)
    {
        pthread_join(workers[i], NULL);
    }
*/
    printf("The output of Matrix C:\n");
    for (i = 0; i < M; i++) //输出得到的矩阵
    {
        for (j = 0; j < N; j++)
        {
            printf("%5d ", C[i][j]);
        }
        printf("\n");
    }
}