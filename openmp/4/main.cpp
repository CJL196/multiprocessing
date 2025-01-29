#include <iostream>
#include "omp.h"

using namespace std;
int add = 0;
static omp_lock_t lock;
int main()
{
    cout << "不使用锁, 加1000次" << endl;
#pragma omp parallel for num_threads(6)
    for (int i = 0; i < 1000; i++)
    {
        add += 1;
    }
    printf("add=%d\n", add);

    cout << "使用锁, 加1000次" << endl;
    add = 0;
    omp_init_lock(&lock); // 初始化互斥锁
#pragma omp parallel for num_threads(6)
    for (int i = 0; i < 1000; i++)
    {
        omp_set_lock(&lock); // 获得锁
        add += 1;
        omp_unset_lock(&lock); // 释放锁
    }
    omp_destroy_lock(&lock); // 销毁锁
    printf("add=%d\n", add);
    return 0;
}