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
    int p[3]; // 用于获取返回值
    pthread_join(id, (void **)&p);
    printf("ret = %d\n", p[0]);
    return (0);
}
