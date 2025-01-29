#include <stdio.h>
#include <omp.h>
int g = 0;
#pragma omp threadprivate(g)
 
int main(int argc, char* argv[])
{
	int t = 20, i;
	#pragma omp parallel num_threads(4)
	{
		g = omp_get_thread_num();
	}
	#pragma omp parallel num_threads(4)
	{
		printf("thread id: %d g: %d\n", omp_get_thread_num(), g);
	}
	return 0;
}
/*
threadprivate子句可以将一个变量复制一个私有的拷贝给各个线程，即各个线程具有各自私有的全局对象

这里threadprivate和private有什么区别呢？private是对每一次并行任务都复制一份变量，threadprivate是对每一个线程复制一份变量。当并行任务数量小于线程数的时候，就相当于给每一个线程都复制一个变量，此时private跟threadprivate效果一样，但是当并行任务数量大于线程数的时候，private和threadprivate的效果就不同了。所以线程私有主要是针对一些面向线程的变量，而不是面向任务的变量。
*/