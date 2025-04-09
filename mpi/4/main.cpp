#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int data;

    // 初始化 MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // 获取当前进程的秩
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 获取进程总数

    // 根进程（rank 0）初始化数据
    if (rank == 0) {
        data = 42;  // 要广播的值
        printf("Process 0 broadcasting data: %d\n", data);
    }

    // 执行广播操作
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 每个进程输出接收到的数据
    printf("Process %d received data: %d\n", rank, data);

    // 结束 MPI
    MPI_Finalize();
    return 0;
}