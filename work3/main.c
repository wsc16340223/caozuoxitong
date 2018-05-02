#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#define NUM_THREADS 10
#define M 3
#define N 3
#define K 2

int A[M][K] = {{1,4}, {2,5}, {3,6}};
int B[K][N] = {{8,7,6}, {5,4,3}};
int C[M][N];

struct v
{
    int i;
    int j;
};

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
    pthread_t workers[NUM_THREADS];

    int i, j, k = 0;
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            struct v *data = (struct v *)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            
            pthread_create(&workers[k], NULL, (void *)runner, (void *)&data);
            
            k++;
        }
    }

    for (i = 0; i < NUM_THREADS - 1; i++)
    {
        pthread_join(workers[i], NULL);
    }

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%3d ", C[i][j]);
        }
        printf("\n");
    }
}