# eBPF介绍

  在Linux-3.15中初步引入eBPF(extended BPF)，Linux-3.18中引入bpf()系统调用，Linux-3.19中引入`SO_ATTACH_BPF`。

> 参考：man bpf

#### 使用eBPF

  使用setsockopt的`SO_ATTACH_BPF`选项，用于注入eBPF程序。

```
    bpf_create_map:
    bpf_prog_load: 
    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    bind(sock, ...);
    setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd, sizeof(prog_fd));
```

#### eBPF的内核实现

  eBPF的实现是基于系统调用bpf:

```

```

#### eBPF虚机

  eBPF虚机是一个RISC CPU，具有11个64-bit Register, 1个PC(Program Counter)和512Byte的Stack。寄存器如下：

  | 寄存器 | 说明 |
  |--------|------|
  | r0     | 存放返回值             |
  | r1-r5  | 函数调用时，存放参数   |
  | r6-r9  |        |
  | r10    | 只读，栈指针(Stack Pointer)    |


