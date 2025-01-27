# SIMD多线程运行器
import multiprocessing
from multiprocessing import Lock
from time import sleep
from tqdm import tqdm

class Logger:
    """输出log"""

    def __init__(self, level="trace", output="stdout"):
        self.output = output
        self.level = level
        assert level in ["error", "warning", "info", "debug", "trace"]
        self.level_map = {"error": 5, "warning": 4, "info": 3, "debug": 2, "trace": 1}
        self.level_num = self.level_map[level]

    def get_log_text_color(self, level, text) -> str:
        if level == "error":
            return f"\x1b[31;1;4m[X] Error\x1b[0m\x1b[31m: {text}\x1b[0m"
        elif level == "warning":
            return f"\x1b[33;1;4m[!] Warning\x1b[0m\x1b[33m: {text}\x1b[0m"
        elif level == "info":
            return f"\x1b[34;1;4m[+] Info\x1b[0m\x1b[34m: {text}\x1b[0m"
        elif level == "debug":
            return f"\x1b[36;1;4m[#] Debug\x1b[0m\x1b[36m: {text}\x1b[0m"
        elif level == "trace":
            return f"{text}"
        else:
            return f"\x1b[31;1;4m[X] Unknown\x1b[0m\x1b[31m: {text}\x1b[0m"

    def get_log_text(self, level, text) -> str:
        if level == "error":
            return f"[X] Error: {text}"
        elif level == "warning":
            return f"[!] Warning: {text}"
        elif level == "info":
            return f"[+] Info: {text}"
        elif level == "debug":
            return f"[#] Debug: {text}"
        elif level == "trace":
            return f"{text}"
        else:
            return f"[X] Unknown: {text}"

    def print(self, level, text):
        if self.output == "stdout":
            print(self.get_log_text_color(level, text))
        else:
            with open(self.output, "a") as f:
                f.write(self.get_log_text(level, text) + "\n")

    def error(self, text):
        self.print("error", text)

    def warning(self, text):
        if self.level_num <= 4:
            self.print("warning", text)

    def info(self, text):
        if self.level_num <= 3:
            self.print("info", text)

    def debug(self, text):
        if self.level_num <= 2:
            self.print("debug", text)

    def trace(self, text):
        if self.level_num <= 1:
            self.print("trace", text)

class SIMD_runner:
    def __init__(self, main_thread_sleep_time=0.01):
        self.sleep_time = main_thread_sleep_time
    
    def run(self, inp_data:list, num_threads:int, worker, logger:Logger=None):
        self.N = len(inp_data)
        self.num_threads = num_threads
        self.mli = multiprocessing.Manager().list()  # 主进程与子进程共享这个字典
        self.mlres = multiprocessing.Manager().list()  # 主进程与子进程共享这个字典
        self.inp_data = inp_data
        # 互斥锁
        self.l = Lock()
        self.simd_worker = self.get_worker(worker)
        self.logger = logger
        
        jobs = [multiprocessing.Process(target=self.simd_worker, args=(self.mli, self.mlres, self.l, i, *self.inp_data[i])) for i in range(self.N)]
        running_job = 0
        next_job = 0
        for j in jobs:
            j.daemon = True # 退出主进程时，子进程也会被自动终止，因为它被设置为守护进程
        # 进度条
        with tqdm(total=self.N, desc="SIMD_runner") as pbar:
            old = 0
            while True:
                self.l.acquire()
                curmli = list(self.mli)
                self.l.release()
                if len(curmli) > old:
                    pbar.update(len(curmli) - old)
                    for i in range(old, len(curmli)):
                        jobs[curmli[i]].join()
                        running_job -= 1
                    old = len(curmli)
                if len(curmli) == self.N:
                    break
                while running_job < num_threads and next_job < self.N:
                    jobs[next_job].start()
                    running_job += 1
                    next_job += 1
                sleep(self.sleep_time)
        mzip = sorted(zip(self.mli, self.mlres))
        mli, mlres = zip(*mzip)
        return list(mlres)
    
    def get_worker(self, calc):
        def SIMD_worker(mli, mlres, l, idx, *data):
            res = calc(*data)
            l.acquire()
            if self.logger is not None:
                self.logger.trace(f"idx={idx}, input={data}, output={res}")
            mli.append(idx)
            mlres.append(res)
            l.release()
        return SIMD_worker

if __name__ == "__main__":
    # 定义worker函数，以输入数据为参数，以输出数据为返回值
    def worker(x, y):
        return x + y, x - y
    N = 100
    # 以列表形式构造输入数据，列表每个元素为元组，元组长度和worker函数输入参数一致
    inp_data = [(i*2, i-3) for i in range(N)]
    # 初始化runner
    runner = SIMD_runner(main_thread_sleep_time=0.01)
    # 每个线程计算完毕后都会借助logger写日志
    logger = Logger(output='test.log')
    # 启动runner
    res = runner.run(inp_data=inp_data, num_threads=4, worker=worker, logger=logger)
    # 打印输出，每个元素为worker函数的返回值，按照输入顺序排序
    print(res)