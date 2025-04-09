#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;
    MPI_Init(&argc, &argv);              // 初始化 MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 获取当前进程的编号

    if (rank == 0) {                     // 小明（进程 0）
        int apples[5] = {1, 2, 3, 4, 5}; // 准备 5 个苹果
        MPI_Send(apples, 5, MPI_INT, 1, 99, MPI_COMM_WORLD); // 寄给小红
        printf("小明: 我寄了 5 个苹果给小红！\n");
    } 
    else if (rank == 1) {                // 小红（进程 1）
        int received_apples[5];          // 准备接收苹果的篮子
        MPI_Recv(received_apples, 5, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 接收
        printf("小红: 我收到小明的苹果：%d, %d, %d, %d, %d\n", 
               received_apples[0], received_apples[1], received_apples[2], 
               received_apples[3], received_apples[4]);
    }

    MPI_Finalize();                      // 关闭 MPI
    return 0;
}