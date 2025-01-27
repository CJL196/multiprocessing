#include <stdio.h>
#include <pthread.h>

int thread(void)
{
    int i;
    for (i = 0; i < 3; i++)
        printf("This is a pthread.\n");
    return 114514;
}

int main(void)
{
    pthread_t id;
    int i, ret;

    // 成功返回0，错误返回错误编号
    ret = pthread_create(&id, NULL, (void *)thread, NULL);
    if (ret != 0)
    {
        printf("Create pthread error!\n");
        exit(1);
    }

    for (i = 0; i < 3; i++)
        printf("This is the main process.\n");

    pthread_detach(id);

    printf("main process exiting\n");
    sleep(1);
    return (0);
}
/*
调用pthread_join(pthread_id)后，如果该线程没有运行结束，调用者会被阻塞，在有些情况下我们并不希望如此，比如在Web服务器中当主线程为每个新来的链接创建一个子线程进行处理的时候，主线程并不希望因为调用pthread_join而阻塞（因为还要继续处理之后到来的链接），这时可以在子线程中加入代码 pthread_detach(pthread_self()) 或者父线程调用 pthread_detach(thread_id)（非阻塞，可立即返回），这将设置子线程的状态为detached, 则子线程运行结束后会自动释放所有资源。
*/
