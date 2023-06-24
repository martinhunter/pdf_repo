
> NOTE: bpf程序都指在内核态运行的bpf程序


### load_bpf_file (deprecated)
1. load_bpf_file
1. do_load_bpf_file
1. load_and_attach
1. bpf_load_program
1. bpf_load_program_name
1. sys_bpf(BPF_PROG_LOAD, &attr, sizeof(attr))

1. SYSCALL_DEFINE3(即sys_bpf)
1. bpf_prog_load

### bpf组件
> bpf依赖的技术：探针

- bpf程序：开发者编写
    - 依赖：clang，llvm编译生成虚拟指令，生成的文件格式为ELF object file
    - 功能：在hook的event触发时，对event数据进行处理
- bpf加载程序：开发者编写
    - 依赖：libbpf库
    - 功能：加载bpf程序，并将bpf程序的函数注册到对应的探针，或从探针处注销
- bpf verifier：已在linux内核代码中
    - 依赖：无
    - 功能：在bpf加载程序加载注册前，检验bpf程序是否符合标准
- bpf virtual machine：已在linux内核代码中
    - 依赖：Jit解释器
    - 功能：event触发时，虚拟机会解释bpf程序的为机器码并运行

### 探针类型
1. hardware events: 硬件探针，是由cpu提供的能力
    1. HPC进行指令数，跳转指令数，cache miss等的计数
    2. LBR记录分支跳转源地址和目的地址，实现调用栈信息记录
2. kernel tracepoint: 静态探针，在kernel源码要被探测的函数里插入探针代码（硬编码），编译出新的内核代码
3. kprobe: 动态探针，动态修改机器码，调用原函数前跳转至hook代码并运行，再跳转回原函数
4. function trace(Ftrace)
    1. 内核编译开启Ftrace功能，编译时每个内核函数第一行会插入nop
    2. 若此时开启function tracer，会将nop行替换为call ftrace_caller，如此会跳转至hook函数
    3. 若此时开启function graph tracer，不仅处理入参，同时处理返回值

### inspect tools
readelf -S hello_kern.o  # 读取节信息
objdump -s hello_kern.o  # 查看字节码信息

llvm-objdump -d hello_kern.o   # 查看反汇编指令
llvm-objdump -r -S -g hello_kern.o   # 查看汇编指令
llvm-readelf -S -r -s -x maps hello_kern.o  # 查看maps数据区

bpf prog show  # 查看正在运行的bpf程序
bpftool prog dump xlated id 95 opcodes  # 显示id95的bpf汇编字节码

### bpf运行过程
1. bpf程序按bpf指令集规范编译成bpf字节码
2. 用户态加载器程序BPF_PROG_LOAD调用bpf_syscall加载bpf字节码
2. 验证器检查字节码
2. 事件触发时，bpf虚拟机调用jit解释器/内核解释器解释bpf字节码生成机器码
    1. 入口bpf_prog_select_runtime
    2. 内核解释器 __bpf_prog_run
    3. JIT解释器  bpf_int_jit_compile

### 程序类型
tracing,通过perf_event_open()创建，结合ioctl()

### 工具
bpf , include/linux/bpf.h #include <linux/bpf.h>
bpf_syscall, kernel/bpf/syscall.c
libbpf库，实际为内核tools/lib/bpf, #include <bpf/xxx>
bpftool工具，tools/bpf/bpftool
bpf_helper辅助函数，tools/include/uapi/linux，bpf程序不能直接调用内核函数，出于兼容性考虑，也出于安全稳定性的考虑，因此内核仅提供少量api供bpf程序访问内核资源/函数
struct bpf_prog，是bpf程序返回的fd
find_sec_def


### bpf程序语法
SEC("tracepoint/syscalls/sys_enter_execvc")
    1. 在libbpf/libbpf.c  section_defs  
    1. tracepoint/<category>/<name>
    1. socket
    1. kprobe




## 框架
### libbpf [git](https://github.com/libbpf/libbpf)
[example](https://www.bilibili.com/video/BV1HW4y1674H/?spm_id_from=333.999.0.0&vd_source=91912fe1eed4fad62dbd9ba27dd52c0e)
[doc](https://libbpf.readthedocs.io/en/latest/libbpf_overview.html#bpf-co-re-compile-once-run-everywhere)
优点：依赖少，性能高，
缺点：效率低，用户态开发困难，难以移植，但通过BPF CO-RE (Compile Once – Run Everywhere)可移植


#### libbpf-bootstrap [git](https://github.com/libbpf/libbpf-bootstrap)

### bcc [git](https://github.com/iovisor/bcc)
优点：最流行，开发效率高，可移植，可动态修改部分代码
缺点：依赖多，运行时编译导致开销大，依赖目标环境头文件

### example
```python

```

#### 编写规则
1. 适当的include
2. 创建BPF_TABLE， BPF_HASH等bpf_map对象
3. 添加要追踪的事件函数
    - 函数名 = '追踪的event' + '\_\_' + '对应可追踪的函数'，如kprobe__*kernel_function_name*
    - 函数名若自定义则需在BPF实例中手动注册
    - 函数的第一个参数（必须）是Registers and BPF context， 剩余参数是要追踪的函数的参数（非必须）
    - 函数中使用BPF_TABLE保存/变更数据
    - 返回0  # ？？？什么时候返回其他
4. BPF类可检测要hook的event函数是否存在
5. BPF实例注册到event
6. BPF实例可通过b\['bpf_map_name'\]来访问bpf_text中创建的map对象

### cilium [git](https://github.com/cilium/ebpf)
优点：开发效率高，是容器技术的bpf开发工具

### lcc [git](https://github.com/aliyun/surftrace/blob/master/setupPylcc.py)
优点：中文文档，封装libbpf，效率高，提供远程（访问pylcc.openanolis.cn启动容器编译创建so文件）和本地eBPF的编译能力以解决难以移植的问题。

### bpftrace
优点：直接通过命令行即可执行
缺点：只适合简单的命令，不适合开发大型项目

### 常见bpf_helpers
u32 pid = bpf_get_current_pid_tgid();  # bcc\src\cc\export\helpers.h  无需include

### hashmap对象
#### BPF_HASH对象操作
- currsock.update(&pid, &sk);
- skpp = currsock.lookup(&pid); if (skpp == 0) return 0;	// missed entry
- currsock.delete(&pid);
#### BPF_TABLE


#### potential bugs
open_perf_buffer依赖online的cpu, down的cpu则会丢失ring buffer?

### 学习资料
- https://space.bilibili.com/518970180/video?tid=0&pn=2&keyword=&order=pubdate
- https://github.com/linuxkerneltravel/lmp

### 名词解释
- HPC: hardware performance counter
- LBR: last branch record
- nop: No Operation是一种指令，也称为空操作指令或空指令。它是一条不执行任何操作的指令，通常用于占位或延迟一段时间
- kprobe: kernel probe 内核态探针
- uprobe: user probe 用户态探针