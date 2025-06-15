#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data, recv_data;

    MPI_Init(&argc, &argv);                 // 初始化MPI环境
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // 获取当前进程编号
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // 获取进程总数

    if (size != 2) {
        if (rank == 0) {
            printf("请用2个进程运行此程序。\n");
        }
        MPI_Finalize();
        return 0;
    }

    send_data = rank + 100; // 每个进程准备要发送的数据

    MPI_Sendrecv(
        &send_data, 1, MPI_INT, 1 - rank, 0,   // 发送到对方进程
        &recv_data, 1, MPI_INT, 1 - rank, 0,   // 从对方进程接收
        MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );

    printf("进程 %d 收到数据 %d\n", rank, recv_data);

    MPI_Finalize(); // 结束MPI环境
    return 0;
}