#include <iostream>
#include "omp.h"

using namespace std;

int main()
{
#pragma omp parallel num_threads(6)
    {
        cout << "Test ";
    }
    cout << endl;
    return 0;
}