#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int sendbuf;             // 每个进程发送的数据
    int recvbuf[4];          // 根进程接收所有数据的缓冲区

    // 初始化 MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // 获取当前进程的秩
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 获取进程总数

    // 每个进程准备要发送的数据
    sendbuf = rank + 1;  // 例如：进程 0 发送 1，进程 1 发送 2，等等

    // 收集所有进程的数据到根进程（rank 0）
    MPI_Gather(&sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 根进程输出收集到的数据
    if (rank == 0) {
        printf("Root process collected data: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", recvbuf[i]);
        }
        printf("\n");
    }

    // 结束 MPI
    MPI_Finalize();
    return 0;
}