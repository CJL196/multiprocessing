#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int sendbuf[4] = {10, 20, 30, 40};  // 根进程中的数据
    int recvbuf;                        // 每个进程接收的数据

    // 初始化 MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // 获取当前进程的秩
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 获取进程总数

    // 确保进程数匹配数据量（这里假设 size = 4）
    if (size != 4) {
        if (rank == 0) {
            printf("This example requires exactly 4 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // 根进程（rank 0）分发数据
    MPI_Scatter(sendbuf, 1, MPI_INT, &recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 每个进程输出接收到的数据
    printf("Process %d received data: %d\n", rank, recvbuf);

    // 结束 MPI
    MPI_Finalize();
    return 0;
}