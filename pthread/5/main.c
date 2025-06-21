#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 5

int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* Thread_work(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);

    printf("Thread %d: before barrier\n", thread_id);

    // Barrier
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == THREAD_COUNT) {
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    } else {
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);

    printf("Thread %d: after barrier\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, Thread_work, thread_id);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
