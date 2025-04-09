#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 每个进程准备一个数字
    int my_number = rank + 1;  // 进程0的数是1，进程1的数是2，以此类推
    int sum = 0;              // 用于存储所有数字的总和

    // 使用 MPI_Allreduce 计算所有数字的总和
    MPI_Allreduce(&my_number, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // 每个进程都打印结果
    printf("进程 %d: 我的数字是 %d，所有数字的总和是 %d\n", 
           rank, my_number, sum);

    MPI_Finalize();
    return 0;
}