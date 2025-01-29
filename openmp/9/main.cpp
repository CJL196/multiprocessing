#include <stdio.h>
#include <omp.h>
int main(int argc, char* argv[])
{
	int t = 20, i;
	#pragma omp parallel for firstprivate(t)
	for (i = 0; i < 5; i++)
	{
        //次数t被初始化为20
		t += i;
		printf("t = %d\n", t);
	}
    //此时t=20
    printf("outside t = %d\n", t);
	return 0;
}
/*
private子句不能继承原变量的值，但是有时我们需要线程私有变量继承原来变量的值，这样我们就可以使用firstprivate子句来实现。
*/