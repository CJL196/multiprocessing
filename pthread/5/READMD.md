# pthread_cond_broadcast 和 pthread_cond_wait 详解

## pthread_cond_wait

- 作用：让线程等待某个条件变量满足，常与互斥锁配合使用。
- 原型：
  ```c
  int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
  ```
- 用法：
  1. 线程先加锁 mutex。
  2. 调用 pthread_cond_wait 后，线程会自动释放 mutex 并进入等待状态。
  3. 被唤醒后，线程会重新获得 mutex 锁。
- 示例：
  ```c
  pthread_mutex_lock(&mutex);
  while (!条件满足) {
      pthread_cond_wait(&cond, &mutex);
  }
  // 条件满足，继续执行
  pthread_mutex_unlock(&mutex);
  ```

## pthread_cond_broadcast

- 作用：唤醒所有等待在指定条件变量上的线程。
- 原型：
  ```c
  int pthread_cond_broadcast(pthread_cond_t *cond);
  ```
- 用法：
  - 通常在修改了共享状态后调用，通知所有等待线程去检查条件。
- 示例：
  ```c
  pthread_mutex_lock(&mutex);
  // 修改共享状态
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&mutex);
  ```

---

这两个函数常用于多线程同步场景，如生产者-消费者模型。