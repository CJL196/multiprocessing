#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int send_data, recv_data;

    // 初始化 MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // 获取当前进程的秩
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 获取进程总数

    // 每个进程提供的数据
    send_data = rank + 1;  // 例如：进程 0 提供 1，进程 1 提供 2，等等

    // 执行归约操作，将所有进程的 send_data 求和，结果存储在进程 0 的 recv_data 中
    MPI_Reduce(&send_data, &recv_data, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // 只有根进程（rank 0）输出结果
    if (rank == 0) {
        printf("Sum of all data: %d\n", recv_data);
    }

    // 结束 MPI
    MPI_Finalize();
    return 0;
}