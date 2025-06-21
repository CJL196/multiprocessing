#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_COUNT 5

/* Shared variables */
int counter = 0;                   // Initialize to 0
sem_t count_sem;                  // Initialize to 1
sem_t barrier_sem;                // Initialize to 0

void* Thread_work(void* arg) {
    int thread_id = *(int*)arg;
    free(arg); // 动态分配的参数需要释放

    printf("Thread %d: before barrier\n", thread_id);

    // Barrier
    sem_wait(&count_sem);
    if (counter == THREAD_COUNT - 1) {
        counter = 0;
        sem_post(&count_sem);
        for (int j = 0; j < THREAD_COUNT - 1; j++) {
            sem_post(&barrier_sem);
        }
    } else {
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }

    printf("Thread %d: after barrier\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    // 初始化信号量
    sem_init(&count_sem, 0, 1);     // 互斥信号量，初值为1
    sem_init(&barrier_sem, 0, 0);   // 屏障信号量，初值为0

    // 创建线程
    for (int i = 0; i < THREAD_COUNT; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, Thread_work, thread_id);
    }

    // 等待线程结束
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);

    return 0;
}
