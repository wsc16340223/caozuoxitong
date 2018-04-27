#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define PERM S_IRUSR | S_IWUSR
#define MAX_SEQUENCE 10

typedef struct
{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(int argc, char**argv)
{
    int i = 0;
    key_t shmid;
    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s\n\a", argv[0]);
        exit(1);
    }
    int size = atoi(argv[1]);  // 获取输入的数组大小
    if (size > MAX_SEQUENCE || size < 0)
    {
        fprintf(stderr, "sequence size must be < %d and >= 0\n", MAX_SEQUENCE);
        exit(1);
    }
    if ((shmid = shmget(IPC_PRIVATE, sizeof(shared_data), PERM)) == -1) //创建共享内存段
    {
        fprintf(stderr, "Create Share Memory Error:%s\n\a", strerror(errno));
        exit(1);
    }
    shared_data* shared_memory = (shared_data *)shmat(shmid, NULL, 0); //加入共享内存
    if (shared_memory == (shared_data *)-1)
    {
        fprintf(stderr, "Unable to detach\n");
    }
    

    shared_memory->sequence_size = size; //将输入的大小转为数组大小

    pid_t pid = fork(); 
    if (pid == 0)
    {
        shared_memory->fib_sequence[0] = 0;
        shared_memory->fib_sequence[1] = 1;
        for (i = 2; i < shared_memory->sequence_size; i++)
        {
            shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i-1] + shared_memory->fib_sequence[i-2];
        }
        shmdt((void *)shared_memory); //分离共享内存段
    }
    else if (pid > 0)
    {
        wait(NULL);
        for (i = 0; i < shared_memory->sequence_size; i++)
        {
            printf("fib: %3d\t%4ld\n", i, shared_memory->fib_sequence[i]);
        }
        shmctl(shmid, IPC_RMID, NULL); // 删除共享内存段
    }
}