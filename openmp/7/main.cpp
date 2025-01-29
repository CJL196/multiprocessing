#include <omp.h>
#include <iostream>
using namespace std;
int main()
{
    omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp single
        {
            cout << "single thread=" << omp_get_thread_num() << endl;
        }
        cout << omp_get_thread_num() << endl;
    }
}
/*
single制导指令所包含的代码段只有一个线程执行，别的线程跳过该代码，如果没有nowait子句，那么其他线程将会在single制导指令结束的隐式同步点等待。有nowait子句其他线程将跳过等待往下执行。
*/