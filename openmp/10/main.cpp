#include <stdio.h>
#include <omp.h>
int main(int argc, char* argv[])
{
	int t = 20, i;
	#pragma omp parallel for firstprivate(t), lastprivate(t)
	for (i = 0; i < 5; i++)
	{
        //次数t被初始化为20
		t += i;
		printf("t = %d\n", t);
	}
    //此时t=24
    printf("outside t = %d\n", t);
	return 0;
}
/*
除了在进入并行部分时需要继承原变量的值外，有时我们还需要在退出并行部分时将计算结果赋值回原变量，lastprivate子句就可以实现这个需求。
需要注意的是，根据OpenMP规范，在循环迭代中，是最后一次迭代的值赋值给原变量；如果是section结构，那么是程序语法上的最后一个section语句赋值给原变量
*/