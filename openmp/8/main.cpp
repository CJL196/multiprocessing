#include <stdio.h>
#include <omp.h>
int main(int argc, char* argv[])
{
	int i = 20;
	#pragma omp parallel for private(i)
	for (i = 0; i < 10; i++)
	{
        printf("i = %d\n", i);
	}
	printf("outside i = %d\n", i);
	return 0;
}
/*
private 子句可以将变量声明为线程私有，声明称线程私有变量以后，每个线程都有一个该变量的副本，线程之间不会互相影响，其他线程无法访问其他线程的副本。原变量在并行部分不起任何作用，也不会受到并行部分内部操作的影响。
*/