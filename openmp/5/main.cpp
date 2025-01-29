#include <omp.h>
#include <iostream>
using namespace std;
int main()
{
    omp_set_num_threads(2);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < 8; i++)
        cout << omp_get_thread_num() << endl;
    cout << "----------" << endl;
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 8; i++)
        cout << omp_get_thread_num() << endl;
}
/*
参考https://zhuanlan.zhihu.com/p/397670985
schedule(type, size)
type参数有四种：1.static, 2.dynamic, 3.guided, 4.runtime

静态调度static: 不用size参数时分配给每个程序的都是n/t次连续迭代，n为迭代次数，t为并行的线程数目。
动态调度dynamic: 先到先得的方式进行任务分配，不用size参数的时候，先把任务干完的线程先取下一个任务，以此类推，而不是一开始就分配固定的任务数。使用size参数的时候，分配的任务以size为单位，一次性分配size个
*/