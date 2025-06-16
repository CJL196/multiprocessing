// 梯形积分
#include <iostream>
#include <mpi.h>

using namespace std;

double f(double x)
{
    return x * x;
}

double calc(double a, double b, int n, double h)
{
    double res = 0;
    for (int i = 0; i < n; ++i)
    {
        res += (f(a + i * h) + f(a + (i + 1) * h)) * h / 2.0;
    }
    return res;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double a = 0.0, b = 10.0;
    int n = 1024;
    if (n % size != 0)
    {
        MPI_Finalize();
        return -1;
    }
    double h = (b - a) / n;
    int local_n = n / size;
    double local_a = rank * local_n * h + a;
    double local_b = (rank + 1) * local_n * h + a;
    double res = calc(local_a, local_b, local_n, h);
    // cout << "rank " << rank << ": " << "local_a=" << local_a << "local_b=" << local_b << " res=" << res << endl;
    if (rank != 0)
    {
        // cout << "rank" << rank << "send" << endl;
        MPI_Send(&res, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
    }
    else
    {
        double tmp = 0;
        for (int i = 1; i < size; ++i)
        {
            // cout << "rank" << i << "receive" << endl;
            MPI_Recv(&tmp, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            res += tmp;
        }
        cout << "res = " << res << endl;
    }
    MPI_Finalize();
    return 0;
}
