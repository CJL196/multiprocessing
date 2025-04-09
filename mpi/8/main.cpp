#include <stdio.h>
#include <mpi.h>

// 定义一个简单的结构体
typedef struct {
    int a;
    float b;
} MyStruct;

int main(int argc, char *argv[]) {
    int rank;
    MyStruct data;
    MPI_Datatype struct_type;

    // 初始化 MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // 准备数据
    if (rank == 0) {
        data.a = 42;
        data.b = 3.14;
    }

    // 定义结构体的组成
    int count = 2;  // 两个字段
    int blocklengths[2] = {1, 1};  // 每个字段的元素数
    MPI_Aint displacements[2];     // 偏移量
    MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};  // 数据类型

    // 计算偏移量
    MPI_Aint base, addr_a, addr_b;
    MPI_Get_address(&data, &base);
    MPI_Get_address(&data.a, &addr_a);
    MPI_Get_address(&data.b, &addr_b);
    displacements[0] = addr_a - base;  // 字段 a 的偏移
    displacements[1] = addr_b - base;  // 字段 b 的偏移

    // 创建自定义类型
    MPI_Type_create_struct(count, blocklengths, displacements, types, &struct_type);

    // 注册类型
    MPI_Type_commit(&struct_type);

    // 进程 0 发送，进程 1 接收
    if (rank == 0) {
        MPI_Send(&data, 1, struct_type, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent: a = %d, b = %.2f\n", data.a, data.b);
    } else if (rank == 1) {
        MPI_Recv(&data, 1, struct_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received: a = %d, b = %.2f\n", data.a, data.b);
    }

    // 释放类型
    MPI_Type_free(&struct_type);

    // 结束 MPI
    MPI_Finalize();
    return 0;
}