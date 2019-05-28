# BPF简介
  最初设计的BPF(Berkeley Packet Filtering)，是用来捕获和过滤网络报文的，参考[The BSD Packet Filter](bpf-usenix93.pdf)。

## 用BPF来抓包
  BPF允许用户空间程序在socket上绑定过滤程序(filter)。例如命令`tcpdump -i em1 tcp dst port 80`，就是要捕获接口em1上，所有目的端口为80的tcp报文：

```
tcpdump -i em1 tcp dst port 80 -d"
  (000) ldh      [12]
  (001) jeq      #0x86dd          jt 2	jf 6
  (002) ldb      [20]
  (003) jeq      #0x6             jt 4	jf 15
  (004) ldh      [56]
  (005) jeq      #0x50            jt 14	jf 15
  (006) jeq      #0x800           jt 7	jf 15
  (007) ldb      [23]
  (008) jeq      #0x6             jt 9	jf 15
  (009) ldh      [20]
  (010) jset     #0x1fff          jt 15	jf 11
  (011) ldxb     4*([14]&0xf)
  (012) ldh      [x + 16]
  (013) jeq      #0x50            jt 14	jf 15
  (014) ret      #262144
  (015) ret      #0
```

> tcpdump的`-d`参数，显示包过滤程序的汇编代码

  libpcap中调用`setsockopt`函数，设置`SO_ATTACH_FILTER`，将过滤程序绑定到socket，`setsockopt`原型如下：

```
  #include <sys/types.h>
  #incldue <sys/socket.h>
  int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```
  使用setsockopt()函数的例子，见[sock_attach](code/sock_attach.c)

  `SO_ATTACH_FILTER`的参数为`struct sock_fprog`，定义于`linux/filter.h`，如下：
```
  struct sock_filter {
    __u16 code;
    __u8 jt;
    __u8 jf;
    __u32 k;
  };

  struct sock_fprog {
    unsigned short len;
    struct sock_filter *filter;
  };
```

  过滤程序使用BPF虚机指令，结构体`struct sock_filer`就是BPF虚机的指令结构。

#### BPF虚机
  BPF虚机是一个RISC CPU，具有11个64-bit Register, 1个PC(Program Counter)和512Byte的Stack。寄存器如下：

  | 寄存器 | 说明 |
  |--------|------|
  | r0     | 存放返回值             |
  | r1-r5  | 函数调用时，存放参数   |
  | r6-r9  |        |
  | r10    | 只读，栈指针(Stack Pointer)    |


