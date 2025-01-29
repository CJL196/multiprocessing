#include <omp.h>
#include <iostream>
using namespace std;
int main()
{
    omp_set_num_threads(3);
#pragma omp parallel sections
    {
#pragma omp section
        {
            cout << omp_get_thread_num();
        }
#pragma omp section
        {
            cout << omp_get_thread_num();
        }
#pragma omp section
        {
            cout << omp_get_thread_num();
        }
    }
}
/*
sections制导指令
用sections把不同的区域交给不同的线程去执行
*/