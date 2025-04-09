# MPI

## MPI_Send

MPI_Send 是 MPI（消息传递接口）中最基本的通信函数之一，用于在一个进程中将数据发送到另一个进程。通俗来说，它就像是寄快递：你把东西（数据）打包好，写上收件人地址（目标进程），然后交给邮局（MPI系统）去送。

下面我详细解释 MPI_Send 的用法，并给出一个通俗易懂的示例。

---

### **MPI_Send 函数的基本语法**

`MPI_Send(void* buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);`

- **buffer**：你要发送的数据（快递包裹的内容）。

- **count**：数据元素的数量（包裹里有多少件东西）。

- **datatype**：数据类型（比如整数、浮点数，告诉MPI怎么理解这些数据）。

- **dest**：目标进程的编号（收件人的地址，用进程的 rank 表示）。

- **tag**：消息的标签（像快递单号，用于区分不同的消息）。

- **comm**：通信器（通常是 MPI_COMM_WORLD，表示所有进程的通信范围）。

---

### **工作原理**

- MPI_Send 是一个**阻塞发送**函数，意思是调用它时，发送方会等待直到数据被安全地发送出去（或者被接收方缓冲）。这就像你寄快递时站在邮局柜台，等着确认包裹被接收。

- 它通常搭配 MPI_Recv 使用，后者是接收数据的函数。



```C
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
```

```Plain Text
mpirun -np 2 ./main
小明: 我寄了 5 个苹果给小红！
小红: 我收到小明的苹果：1, 2, 3, 4, 5
```

### **扩展：非阻塞版本**

如果不想等快递员确认，可以用 MPI_Isend（非阻塞发送），就像寄了包裹就走人，不等对方签收。不过这需要额外的管理（用 MPI_Wait 检查是否送达），这里就不展开了。

## MPI_Recv

`MPI_Recv` 是 MPI（消息传递接口）中用于接收数据的核心函数，与 `MPI_Send` 配对使用。通俗来说，它就像是收快递：你站在家门口等着快递员（发送进程）把包裹（数据）送到你手上，然后拆开看看里面是什么。

下面我详细解释 `MPI_Recv` 的用法，并给出一个简单易懂的示例。

---

### **MPI_Recv 函数的基本语法**

```C
MPI_Recv(void* buffer, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status);
```

- **`buffer`**：接收数据的缓冲区（你准备的“篮子”或“箱子”用来装收到的东西）。

- **`count`**：期望接收的数据元素数量（篮子能装多少件东西）。

- **`datatype`**：数据类型（比如 `MPI_INT` 或 `MPI_DOUBLE`，告诉 MPI 怎么解析数据）。

- **`source`**：发送方的进程编号（谁寄来的快递，用进程的 `rank` 表示）。

- **`tag`**：消息的标签（快递单号，用于匹配发送的消息）。

- **`comm`**：通信器（通常是 `MPI_COMM_WORLD`，表示所有进程的通信范围）。

- **`status`**：状态对象（可以记录接收到的消息详情，比如实际来源或数据量），可以用 `MPI_STATUS_IGNORE` 忽略。

---

### **工作原理**

- `MPI_Recv` 是一个**阻塞接收**函数，调用它时，进程会等待直到指定的消息到达。如果发送方还没发送数据，接收方会一直等着，就像你在家等快递但快递员还没到。

- 它必须与 `MPI_Send` 的参数（`count`、`datatype`、`tag` 等）匹配，否则可能收不到数据或出错。

---

### **通俗示例：收包裹**

假设有两个进程：小明（`rank 0`）和小红（`rank 1`）。小明给小红寄了 3 个玩具（数据），小红用 `MPI_Recv` 接收。

#### **代码实现**

```C
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;
    MPI_Init(&argc, &argv);              // 初始化 MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 获取当前进程的编号

    if (rank == 0) {                       // 小明（进程 0）
        int toys[3] = {1, 2, 3};           // 准备 3 个玩具
        MPI_Send(toys, 3, MPI_INT, 1, 123, MPI_COMM_WORLD); // 寄给小红
        printf("小明: 我寄了 3 个玩具给小红！\n");
    } 
    else if (rank == 1) {                  // 小红（进程 1）
        int received_toys[3];              // 准备一个空数组接收玩具
        MPI_Recv(received_toys, 3, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 接收
        printf("小红: 我收到小明的玩具：%d, %d, %d\n", 
               received_toys[0], received_toys[1], received_toys[2]);
    }

    MPI_Finalize();                        // 关闭 MPI
    return 0;
}
```

#### **解释代码**

1. **小明（进程 0）**：

    - 用 `MPI_Send` 发送一个包含 3 个整数的数组 `toys` 给小红（`dest = 1`），标签是 123。

1. **小红（进程 1）**：

    - 准备一个空数组 `received_toys` 来接收数据。

    - 用 `MPI_Recv` 从小明（`source = 0`）接收 3 个整数，标签是 123。

    - 接收后，打印收到的玩具。

1. **运行结果**（用 `mpirun -np 2 ./program` 运行）：

    ```Plain Text
    小明: 我寄了 3 个玩具给小红！
    小红: 我收到小明的玩具：1, 2, 3
    ```

---

### **注意事项**

1. **参数匹配**：

    - `count`、`datatype` 和 `tag` 必须与发送方的 `MPI_Send` 一致。比如，小明寄了 3 个整数，小红就得告诉 `MPI_Recv` 接收 3 个 `MPI_INT`，不然会出错。

1. **阻塞特性**：

    - 如果小明还没寄包裹（没调用 `MPI_Send`），小红会一直卡在 `MPI_Recv` 那里等着，就像你等快递但物流还没动。

1. **`source` 的灵活性**：

    - 可以用 `MPI_ANY_SOURCE` 代替具体的 `source`，表示“随便谁寄来的我都收”，但要小心确保程序逻辑正确。

1. **`tag` 的灵活性**：

    - 可以用 `MPI_ANY_TAG` 代替具体的 `tag`，表示“不管单号是啥我都收”。

1. **`status` 参数**：

    - 如果用 `MPI_STATUS_IGNORE`，就不关心额外信息。如果需要知道实际收到的数据量或来源，可以定义一个 `MPI_Status` 变量。

---

### **扩展：用 `status` 查看详情**

如果小红不确定小明寄了多少东西，可以用 `MPI_Recv` 搭配 `MPI_Get_count` 检查实际接收的数据量：

```C
else if (rank == 1) {
    int received_toys[10]; // 准备一个大点的篮子
    MPI_Status status;
    MPI_Recv(received_toys, 10, MPI_INT, 0, 123, MPI_COMM_WORLD, &status); // 接收
    int count;
    MPI_Get_count(&status, MPI_INT, &count); // 获取实际收到的数量
    printf("小红: 我收到 %d 个玩具\n", count);
}
```

这里即使小红准备接收 10 个玩具，但小明只寄了 3 个，`count` 会告诉小红实际收到 3 个。

---

### **更贴近生活的比喻**

- **`MPI_Recv` 在主从模式**：员工（从进程）等着老板（主进程）发任务，收到后开工。

- **`MPI_Recv` 在对等模式**：朋友之间互相寄礼物，谁寄来我就收谁的。

---

### **非阻塞版本**

如果不想一直等着快递，可以用 `MPI_Irecv`（非阻塞接收），相当于告诉快递员“放门口就行，我晚点自己拿”，然后用 `MPI_Wait` 检查是否送达。

## MPI_Reduce

`MPI_Reduce` 是 MPI（Message Passing Interface）中用于数据归约操作的函数，常用于并行计算中。它可以将多个进程中的数据按照指定的操作（如求和、求最大值等）归约到一个目标进程中。以下是 `MPI_Reduce` 的基本用法和说明：

### 函数原型

```C
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, 
               MPI_Op op, int root, MPI_Comm comm);
```

### 参数说明

1. **`sendbuf`**

    - 输入参数：每个进程提供的发送缓冲区，包含要归约的数据。

    - 类型：`const void *`（指向数据的指针）。

1. **`recvbuf`**

    - 输出参数：目标进程接收归约结果的缓冲区。

    - 类型：`void *`。

    - 注：只有根进程（`root`）的 `recvbuf` 会被填充结果，其他进程的 `recvbuf` 无意义。

1. **`count`**

    - 每个进程发送缓冲区中的数据元素个数。

    - 类型：`int`。

1. **`datatype`**

    - 数据元素的类型，例如 `MPI_INT`（整数）、`MPI_FLOAT`（浮点数）、`MPI_DOUBLE` 等。

    - 类型：`MPI_Datatype`。

1. **`op`**

    - 归约操作类型，例如 `MPI_SUM`（求和）、`MPI_MAX`（最大值）、`MPI_MIN`（最小值）、`MPI_PROD`（乘积）等。

    - 类型：`MPI_Op`。

1. **`root`**

    - 接收归约结果的进程的秩（rank）。

    - 类型：`int`。

1. **`comm`**

    - 通信域，通常是 `MPI_COMM_WORLD`，表示参与归约的进程集合。

    - 类型：`MPI_Comm`。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 失败时返回错误码。

### 使用步骤

1. 初始化 MPI 环境：
使用 `MPI_Init` 初始化 MPI。

2. 获取进程信息：
使用 `MPI_Comm_rank` 获取当前进程的秩，`MPI_Comm_size` 获取进程总数。

3. 准备数据：
每个进程准备好要归约的数据（放在 `sendbuf` 中）。

4. 调用 `MPI_Reduce`：
执行归约操作，结果存储在根进程的 `recvbuf` 中。

5. 清理：
使用 `MPI_Finalize` 结束 MPI 环境。

### 示例代码

以下是一个简单的 C 示例，计算所有进程中数据的总和，并将结果存储在进程 0 中：

```C
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
```

### 运行示例

假设有 4 个进程（`size = 4`），每个进程的 `send_data` 分别是：

- 进程 0：1

- 进程 1：2

- 进程 2：3

- 进程 3：4

运行后，根进程（rank 0）的 `recv_data` 将是 `1 + 2 + 3 + 4 = 10`，输出：

```Plain Text
Sum of all data: 10
```

### 注意事项

1. **数据一致性**：所有进程的 `count` 和 `datatype` 必须一致，否则可能导致错误。

2. **缓冲区大小**：`sendbuf` 和 `recvbuf` 的大小必须足够容纳 `count` 个 `datatype` 类型的数据。

3. **非根进程**：非根进程的 `recvbuf` 参数会被忽略，但仍需提供合法的指针（可以是 `NULL`，但不推荐）。

4. **同步性**：`MPI_Reduce` 是一个阻塞操作，所有参与进程必须调用它才会继续执行。

### 常见归约操作

- `MPI_SUM`：求和

- `MPI_PROD`：求积

- `MPI_MAX`：求最大值

- `MPI_MIN`：求最小值

- `MPI_LAND`：逻辑与

- `MPI_LOR`：逻辑或

## MPI_Allreduce

`MPI_Allreduce` 是 MPI（Message Passing Interface）中另一种归约操作函数，与 `MPI_Reduce` 类似，但它的区别在于：归约结果不仅发送到根进程，而是广播到所有参与的进程。也就是说，调用 `MPI_Allreduce` 后，每个进程都会收到相同的归约结果。以下是它的使用方法和详细说明：

### 函数原型

```C
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, 
                  MPI_Op op, MPI_Comm comm);
```

### 参数说明

1. **`sendbuf`**

    - 输入参数：每个进程提供的发送缓冲区，包含要归约的数据。

    - 类型：`const void *`。

1. **`recvbuf`**

    - 输出参数：每个进程接收归约结果的缓冲区。

    - 类型：`void *`。

    - 注：与 `MPI_Reduce` 不同，所有进程的 `recvbuf` 都会被填充相同的结果。

1. **`count`**

    - 每个进程发送缓冲区中的数据元素个数。

    - 类型：`int`。

1. **`datatype`**

    - 数据元素的类型，例如 `MPI_INT`（整数）、`MPI_FLOAT`（浮点数）、`MPI_DOUBLE` 等。

    - 类型：`MPI_Datatype`。

1. **`op`**

    - 归约操作类型，例如 `MPI_SUM`（求和）、`MPI_MAX`（最大值）、`MPI_MIN`（最小值）、`MPI_PROD`（乘积）等。

    - 类型：`MPI_Op`。

1. **`comm`**

    - 通信域，通常是 `MPI_COMM_WORLD`，表示参与归约的进程集合。

    - 类型：`MPI_Comm`。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 失败时返回错误码。

### 与 `MPI_Reduce` 的主要区别

- `MPI_Reduce`：结果只发送到指定的根进程（`root`），其他进程的 `recvbuf` 无意义。

- `MPI_Allreduce`：没有 `root` 参数，结果广播到所有进程，每个进程的 `recvbuf` 都会得到相同的结果。

### 示例代码

```C
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
```

```C
进程 0: 我的数字是 1，所有数字的总和是 10
进程 1: 我的数字是 2，所有数字的总和是 10
进程 2: 我的数字是 3，所有数字的总和是 10
进程 3: 我的数字是 4，所有数字的总和是 10
```

### 使用场景

- 当所有进程都需要知道归约结果时（例如全局误差计算、同步参数更新），使用 `MPI_Allreduce`。

- 如果只需要一个进程知道结果（例如收集统计数据），则更适合用 `MPI_Reduce`。

如果你有更具体的需求（例如自定义操作或性能优化），可以告诉我，我会进一步帮你完善代码或解答问题！

## MPI_Bcast

`MPI_Bcast` 是 MPI（Message Passing Interface）中的广播函数，用于将数据从一个进程（称为根进程）广播到通信域中的所有其他进程。它在并行计算中非常常用，例如需要同步参数或分发初始数据时。以下是 `MPI_Bcast` 的使用方法和详细说明：

### 函数原型

```C
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
```

### 参数说明

1. **`buffer`**

    - 输入/输出参数：数据的缓冲区。

    - 类型：`void *`。

    - 注：在根进程中，`buffer` 是要广播的源数据；在其他进程中，`buffer` 是接收数据的目标缓冲区。

1. **`count`**

    - 缓冲区中的数据元素个数。

    - 类型：`int`。

1. **`datatype`**

    - 数据元素的类型，例如 `MPI_INT`（整数）、`MPI_FLOAT`（浮点数）、`MPI_DOUBLE` 等。

    - 类型：`MPI_Datatype`。

1. **`root`**

    - 广播源进程的秩（rank），即发送数据的进程。

    - 类型：`int`。

1. **`comm`**

    - 通信域，通常是 `MPI_COMM_WORLD`，表示参与广播的进程集合。

    - 类型：`MPI_Comm`。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 失败时返回错误码。

### 使用步骤

1. 初始化 MPI 环境：
使用 `MPI_Init` 初始化 MPI。

2. 获取进程信息：
使用 `MPI_Comm_rank` 获取当前进程的秩，`MPI_Comm_size` 获取进程总数。

3. 准备数据：
根进程准备要广播的数据，存放在 `buffer` 中；其他进程准备接收缓冲区。

4. 调用 `MPI_Bcast`：
执行广播操作，所有进程的 `buffer` 都会接收到根进程的数据。

5. 清理：
使用 `MPI_Finalize` 结束 MPI 环境。

### 示例代码

以下是一个简单的 C 示例，将根进程（rank 0）中的一个整数广播到所有进程：

```C
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
```

### 运行示例

假设有 4 个进程（`size = 4`），根进程（rank 0）广播的值是 `42`。运行后，输出类似：

```C
Process 0 broadcasting data: 42
Process 0 received data: 42
Process 1 received data: 42
Process 2 received data: 42
Process 3 received data: 42
```

### 注意事项

1. **数据一致性**：所有进程的 `count` 和 `datatype` 必须一致，否则可能导致错误或未定义行为。

2. **缓冲区大小**：`buffer` 的大小必须足够容纳 `count` 个 `datatype` 类型的数据。

3. **同步性**：`MPI_Bcast` 是一个阻塞操作，所有参与进程必须调用它才会继续执行。

4. **根进程**：根进程的 `buffer` 是只读的（广播源），其他进程的 `buffer` 会被覆盖。

5. **内存分配**：非根进程调用 `MPI_Bcast` 前，必须确保 `buffer` 已分配足够的空间。

### 使用场景

- **分发全局参数**：例如将配置文件或初始值广播给所有进程。

- **同步数据**：确保所有进程使用相同的数据开始计算。

- **简化通信**：相比点对点通信（如 `MPI_Send` 和 `MPI_Recv`），`MPI_Bcast` 更高效地完成一对多通信。

### 与其他函数的对比

- **`MPI_Bcast` vs `MPI_Reduce`**：

    - `MPI_Bcast` 是从一个进程广播到所有进程。

    - `MPI_Reduce` 是从所有进程归约到单一进程。

- **`MPI_Bcast` vs `MPI_Allreduce`**：

    - `MPI_Bcast` 只广播数据，不进行计算。

    - `MPI_Allreduce` 结合归约和广播，结果分发给所有进程。

如果你有更具体的需求（例如广播数组、结构体，或优化通信），可以告诉我，我会进一步帮你完善代码或解答疑问！

## MPI_Scatter

`MPI_Scatter` 是 MPI（Message Passing Interface）中的数据分发函数，用于将根进程中的数据分成若干部分，并将每部分发送到通信域中的不同进程。它在并行计算中非常有用，例如将大数组分块分配给多个进程进行处理。以下是 `MPI_Scatter` 的使用方法和详细说明：

### 函数原型

```C
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm);
```

### 参数说明

1. **`sendbuf`**

    - 输入参数：根进程中要分发的数据缓冲区。

    - 类型：`const void *`。

    - 注：只有根进程的 `sendbuf` 有意义，其他进程可以传入 `NULL`。

1. **`sendcount`**

    - 每个进程接收的数据元素个数（从根进程的角度看，每次发送的块大小）。

    - 类型：`int`。

1. **`sendtype`**

    - 发送数据的类型，例如 `MPI_INT`（整数）、`MPI_FLOAT`（浮点数）、`MPI_DOUBLE` 等。

    - 类型：`MPI_Datatype`。

1. **`recvbuf`**

    - 输出参数：每个进程接收数据的缓冲区。

    - 类型：`void *`。

1. **`recvcount`**

    - 每个进程接收的数据元素个数。

    - 类型：`int`。

    - 注：通常与 `sendcount` 相等，除非使用特殊数据类型。

1. **`recvtype`**

    - 接收数据的类型，通常与 `sendtype` 相同。

    - 类型：`MPI_Datatype`。

1. **`root`**

    - 发送数据的根进程的秩（rank）。

    - 类型：`int`。

1. **`comm`**

    - 通信域，通常是 `MPI_COMM_WORLD`，表示参与分发的进程集合。

    - 类型：`MPI_Comm`。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 失败时返回错误码。

### 使用步骤

1. 初始化 MPI 环境：
使用 `MPI_Init` 初始化 MPI。

2. 获取进程信息：
使用 `MPI_Comm_rank` 获取当前进程的秩，`MPI_Comm_size` 获取进程总数。

3. 准备数据：
根进程准备要分发的完整数据（`sendbuf`），其他进程准备接收缓冲区（`recvbuf`）。

4. 调用 `MPI_Scatter`：
将根进程的数据分发到所有进程，每个进程接收一部分。

5. 清理：
使用 `MPI_Finalize` 结束 MPI 环境。

### 示例代码

以下是一个简单的 C 示例，将根进程中的一个整数数组分发给所有进程：

```C
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
```

### 运行示例

假设有 4 个进程（`size = 4`），根进程（rank 0）的 `sendbuf` 是 `{10, 20, 30, 40}`。运行后，输出类似：

```C
Process 0 received data: 10
Process 1 received data: 20
Process 2 received data: 30
Process 3 received data: 40
```

### 数据分发原理

- 根进程的 `sendbuf` 被分成 `size` 份（`size` 是进程总数）。

- 每份的大小由 `sendcount` 指定。

- 第 `i` 个进程（rank = i）接收 `sendbuf[i * sendcount]` 开始的 `sendcount` 个元素。

### 注意事项

1. **数据一致性**：

    - `sendcount` 和 `recvcount` 通常相等，且 `sendtype` 与 `recvtype` 一致。

    - 根进程的 `sendbuf` 总大小应至少为 `sendcount * size`，否则可能导致缓冲区溢出或未定义行为。

1. **缓冲区大小**：

    - 每个进程的 `recvbuf` 必须足够容纳 `recvcount` 个 `recvtype` 类型的数据。

1. **同步性**：

    - `MPI_Scatter` 是一个阻塞操作，所有参与进程必须调用它才会继续执行。

1. **非根进程**：

    - 非根进程的 `sendbuf` 参数被忽略，可以传入 `NULL`。

### 使用场景

- **数据并行**：将大数组分块分配给多个进程进行并行计算，例如矩阵分解或图像处理。

- **任务分配**：将任务参数分发给各个工作进程。

### 与其他函数的对比

- **`MPI_Scatter` vs `MPI_Bcast`**：

    - `MPI_Scatter` 将数据分成多份，每进程接收不同部分。

    - `MPI_Bcast` 将相同的数据广播给所有进程。

- **`MPI_Scatter` vs `MPI_Gather`**：

    - `MPI_Scatter` 是从根进程分发数据到所有进程。

    - `MPI_Gather` 是从所有进程收集数据到根进程。

如果你有更具体的需求（例如分发多维数组或动态数据），可以告诉我，我会进一步帮你优化代码或解答疑问！

## MPI_Gather

`MPI_Gather` 是 MPI（Message Passing Interface）中的数据收集函数，用于将所有进程中的数据收集到根进程。它与 `MPI_Scatter` 的功能相反，适用于需要从多个进程汇总数据的场景，例如收集计算结果或统计信息。以下是 `MPI_Gather` 的使用方法和详细说明：

### 函数原型

```C
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm);
```

### 参数说明

1. **`sendbuf`**

    - 输入参数：每个进程要发送的数据缓冲区。

    - 类型：`const void *`。

1. **`sendcount`**

    - 每个进程发送的数据元素个数。

    - 类型：`int`。

1. **`sendtype`**

    - 发送数据的类型，例如 `MPI_INT`（整数）、`MPI_FLOAT`（浮点数）、`MPI_DOUBLE` 等。

    - 类型：`MPI_Datatype`。

1. **`recvbuf`**

    - 输出参数：根进程接收所有数据的缓冲区。

    - 类型：`void *`。

    - 注：只有根进程的 `recvbuf` 有意义，其他进程可以传入 `NULL`。

1. **`recvcount`**

    - 根进程从每个进程接收的数据元素个数。

    - 类型：`int`。

    - 注：通常与 `sendcount` 相等，除非使用特殊数据类型。

1. **`recvtype`**

    - 接收数据的类型，通常与 `sendtype` 相同。

    - 类型：`MPI_Datatype`。

1. **`root`**

    - 接收数据的根进程的秩（rank）。

    - 类型：`int`。

1. **`comm`**

    - 通信域，通常是 `MPI_COMM_WORLD`，表示参与收集的进程集合。

    - 类型：`MPI_Comm`。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 失败时返回错误码。

### 使用步骤

1. 初始化 MPI 环境：
使用 `MPI_Init` 初始化 MPI。

2. 获取进程信息：
使用 `MPI_Comm_rank` 获取当前进程的秩，`MPI_Comm_size` 获取进程总数。

3. 准备数据：
每个进程准备要发送的数据（`sendbuf`），根进程准备接收缓冲区（`recvbuf`）。

4. 调用 `MPI_Gather`：
将所有进程的数据收集到根进程的 `recvbuf` 中。

5. 清理：
使用 `MPI_Finalize` 结束 MPI 环境。

### 示例代码

以下是一个简单的 C 示例，收集每个进程中的一个整数到根进程：

```C
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
```

### 运行示例

假设有 4 个进程（`size = 4`），每个进程的 `sendbuf` 分别是：

- 进程 0：1

- 进程 1：2

- 进程 2：3

- 进程 3：4

运行后，根进程（rank 0）的 `recvbuf` 将包含 `{1, 2, 3, 4}`，输出类似：

```C
Root process collected data: 1 2 3 4
```

### 数据收集原理

- 每个进程发送 `sendcount` 个 `sendtype` 类型的数据。

- 根进程的 `recvbuf` 按进程秩顺序接收数据：

    - `recvbuf[0]` 到 `recvbuf[recvcount-1]` 来自 rank 0。

    - `recvbuf[recvcount]` 到 `recvbuf[2*recvcount-1]` 来自 rank 1。

    - 依此类推。

### 注意事项

1. **数据一致性**：

    - `sendcount` 和 `recvcount` 通常相等，且 `sendtype` 与 `recvtype` 一致。

    - 根进程的 `recvbuf` 总大小应至少为 `recvcount * size`，否则可能导致缓冲区溢出。

1. **缓冲区大小**：

    - 每个进程的 `sendbuf` 必须足够容纳 `sendcount` 个 `sendtype` 类型的数据。

    - 根进程的 `recvbuf` 必须足够容纳所有进程的数据。

1. **同步性**：

    - `MPI_Gather` 是一个阻塞操作，所有参与进程必须调用它才会继续执行。

1. **非根进程**：

    - 非根进程的 `recvbuf` 参数被忽略，可以传入 `NULL`。

### 使用场景

- **结果汇总**：收集每个进程的计算结果，例如并行求和或统计。

- **数据合并**：将分布式数据集中到单一进程进行后续处理。

### 与其他函数的对比

- **`MPI_Gather` vs `MPI_Scatter`**：

    - `MPI_Gather` 是从所有进程收集数据到根进程。

    - `MPI_Scatter` 是从根进程分发数据到所有进程。

- **`MPI_Gather` vs `MPI_Reduce`**：

    - `MPI_Gather` 只收集数据，不进行计算。

    - `MPI_Reduce` 收集并对数据进行归约操作（如求和、求最大值）。

如果你有更具体的需求（例如收集多维数组或动态数据），可以告诉我，我会进一步帮你优化代码或解答疑问！

## MPI_Allgather

`MPI_Allgather` 是 MPI（Message Passing Interface）中的一个集体通信函数，用于将每个进程的数据收集到所有进程中。换句话说，它会把每个进程的本地数据分发给所有其他进程，最终每个进程都会拥有所有进程的数据副本。以下是它的使用方法和相关说明：

### 函数原型

```C
int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm);
```

### 参数说明

1. **`sendbuf`**
指向发送数据的缓冲区（每个进程提供的数据）。这是每个进程要贡献给全局结果的数据。

2. **`sendcount`**
每个进程发送的数据元素数量（基于 `sendtype` 定义的类型）。

3. **`sendtype`**
发送数据的 MPI 数据类型（如 `MPI_INT`、`MPI_FLOAT` 等）。

4. **`recvbuf`**
接收缓冲区，用于存储所有进程发送的数据。调用结束后，每个进程的 `recvbuf` 都会包含所有进程的数据。

5. **`recvcount`**
每个进程接收的单个数据块的元素数量（通常等于 `sendcount`）。

6. **`recvtype`**
接收数据的 MPI 数据类型（通常与 `sendtype` 相同）。

7. **`comm`**
MPI 通信器（如 `MPI_COMM_WORLD`），定义参与通信的进程组。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 出错时返回错误码。

### 使用注意

- **`recvbuf`** 的大小必须足够容纳所有进程的数据，即 `recvcount * 进程数` 个元素。

- 所有进程必须调用 `MPI_Allgather`，且参数（如 `sendcount` 和 `recvcount`）在所有进程中应一致。

- 与 `MPI_Gather` 不同，`MPI_Allgather` 没有“根进程”的概念，所有进程都会收到完整的数据。

### 示例代码

以下是一个简单的 C 示例，展示如何使用 `MPI_Allgather`：

```C
#include <stdio.h>
#include <mpi.h>

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
```

### 示例运行结果

假设有 4 个进程（`size = 4`），运行后输出可能如下：

```C
Process 0 received data: 1 2 3 4
Process 1 received data: 1 2 3 4
Process 2 received data: 1 2 3 4
Process 3 received data: 1 2 3 4
```

每个进程都收到了所有进程的数据 `[1, 2, 3, 4]`，其中 `1` 来自进程 0，`2` 来自进程 1，依此类推。

### 常见用途

- 数据分发：当所有进程都需要一份全局数据副本时。

- 并行计算：例如，在矩阵运算或分布式数组操作中同步数据。

如果你有具体问题或需要更复杂的例子，可以告诉我，我会进一步帮你优化或解释！

## MPI_Type_create_struct

`MPI_Type_create_struct` 是 MPI（Message Passing Interface）中用于创建自定义结构化数据类型的函数。它允许你定义一个复杂的数据类型（比如 C 中的结构体），以便在 MPI 通信中传递非连续或异构的数据。下面是它的使用方法和详细说明：

### 函数原型

```C
int MPI_Type_create_struct(int count,
                           const int array_of_blocklengths[],
                           const MPI_Aint array_of_displacements[],
                           const MPI_Datatype array_of_types[],
                           MPI_Datatype *newtype);
```

### 参数说明

1. **`count`**
结构中数据块的数量（即结构体的字段数）。

2. **`array_of_blocklengths`**
每个数据块的元素数量数组。例如，如果某个字段是一个包含 3 个整数的数组，这里就填 3。

3. **`array_of_displacements`**
每个数据块在内存中相对于结构体起始地址的字节偏移量数组。通常使用 `MPI_Aint` 类型表示（地址整数类型）。

4. **`array_of_types`**
每个数据块的 MPI 数据类型数组（如 `MPI_INT`、`MPI_FLOAT` 等）。

5. **`newtype`**
输出参数，返回新创建的 MPI 数据类型。需要后续调用 `MPI_Type_commit` 来注册这个类型才能使用。

### 返回值

- 成功时返回 `MPI_SUCCESS`。

- 出错时返回错误码。

### 使用步骤

1. **定义结构体和偏移量**：计算每个字段的内存偏移量（可以用 `MPI_Get_address` 获取）。

2. **创建数据类型**：调用 `MPI_Type_create_struct` 创建自定义类型。

3. **注册类型**：调用 `MPI_Type_commit` 激活新类型。

4. **使用类型**：在通信函数（如 `MPI_Send`、`MPI_Recv`）中使用。

5. **释放类型**：使用完成后调用 `MPI_Type_free` 释放。

### 示例代码

以下是一个示例，展示如何用 `MPI_Type_create_struct` 定义一个包含整数和浮点数的结构体，并进行通信：

```C
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
```

### 示例运行结果

假设运行在两个进程上，输出可能是：

```C
Process 0 sent: a = 42, b = 3.14
Process 1 received: a = 42, b = 3.14
```

### 关键点

1. **偏移量计算**：

    - 使用 `MPI_Get_address` 获取字段的绝对地址，然后计算相对偏移量。

    - 偏移量必须精确，否则会导致数据错误。

1. **内存对齐**：

    - C 结构体可能有填充字节（padding），`MPI_Type_create_struct` 会根据提供的偏移量处理，不受填充影响。

1. **类型提交**：

    - 调用 `MPI_Type_commit` 是必需的，否则新类型无法使用。

1. **适用场景**：

    - 用于传输复杂数据结构，如结构体、嵌套数组等。

### 注意事项

- 确保所有进程使用的 `struct_type` 定义一致。

- 如果结构体中包含指针，`MPI_Type_create_struct` 无法直接处理指针指向的内容，需要单独处理。

如果你有更复杂的结构体或具体问题，可以提供更多细节，我会帮你进一步优化代码或解答！

