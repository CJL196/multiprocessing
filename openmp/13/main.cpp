#include <stdio.h>
#include <omp.h>
static omp_lock_t lock;
int main(int argc, char* argv[])
{
    omp_init_lock(&lock); // 初始化互斥锁
	int t = 20, i;
	#pragma omp parallel for private(t)
	for (i = 0; i < 10; i++)
	{
        omp_set_lock(&lock); // 获得锁
		if (i % 2 == 0)
			t++;
		printf("i = %d, t = %d\n", i, t);
        omp_unset_lock(&lock); // 释放锁
	}
    omp_destroy_lock(&lock); // 销毁锁
    printf("t=%d\n", t);
    return 0;
}