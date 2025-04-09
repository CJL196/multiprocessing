#include <stdio.h>
#include <mpi.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data;
    int *recv_data;

    // 初始化 MPI 环境
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // 获取当前进程的 rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 获取进程总数

    // 每个进程准备自己的数据
    send_data = rank + 1;  // 例如，进程 0 提供 1，进程 1 提供 2，等等

    // 分配接收缓冲区，足以容纳所有进程的数据
    recv_data = (int *)malloc(size * sizeof(int));

    // 调用 MPI_Allgather
    MPI_Allgather(&send_data, 1, MPI_INT,    // 发送 1 个整数
                  recv_data, 1, MPI_INT,     // 每个进程接收 1 个整数
                  MPI_COMM_WORLD);

    // 输出结果
    printf("Process %d received data: ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    // 清理并结束
    free(recv_data);
    MPI_Finalize();
    return 0;
}