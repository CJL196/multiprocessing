#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#define MAX 10

pthread_t thread[2];
pthread_mutex_t mut;
int number = 0;

void thread1()
{
    printf("thread1 : I'm thread 1,starting...\n");

    for (; number < MAX;)
    {
        printf("thread1 : number = %d\n", number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(1);
    }

    pthread_exit(NULL);
}

void thread2()
{
    printf("thread2 : I'm thread 2,starting...\n");

    for (; number < MAX;)
    {
        printf("thread2 : number = %d\n", number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(1);
    }

    pthread_exit(NULL);
}

void thread_create(void)
{
    int temp;
    memset(&thread, 0, sizeof(thread)); // comment1
    /*创建线程*/
    if ((temp = pthread_create(&thread[0], NULL, (void *)thread1, NULL)) != 0) // comment2
        printf("create thread1 failed!\n");
    else
        printf("create thread1 successed!\n");

    if ((temp = pthread_create(&thread[1], NULL, (void *)thread2, NULL)) != 0) // comment3
        printf("create thread failed!\n");
    else
        printf("create thread2 successed!\n");
}

void thread_wait(void)
{
    /*等待线程结束*/
    if (thread[0] != 0)
    {
        // comment4
        pthread_join(thread[0], NULL);
        printf("thread1 end !\n");
    }
    if (thread[1] != 0)
    {
        // comment5
        pthread_join(thread[1], NULL);
        printf("thread2 end !\n");
    }
}

int main()
{
    /*用默认属性初始化互斥锁*/
    pthread_mutex_init(&mut, NULL);
    thread_create();
    thread_wait();

    return 0;
}
