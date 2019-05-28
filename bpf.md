# BPF简介
  最初设计的BPF(Berkeley Packet Filtering)，是用来捕获和过滤网络报文的，参考[The BSD Packet Filter](bpf-usenix93.pdf)。

## 用BPF来抓包

  tcpdump是最常用的抓包工具，可以在命令行中指定过滤规则，例如`tcpdump -i em1 tcp dst port 80`，就是要捕获接口em1上，所有目的端口为80的tcp报文。tcpdump的过滤规则，可以被翻译为过滤程序。用tcpdump的`-d`参数，显示过滤程序的汇编代码，`-dd`参数显示过滤程序的Bytecode：
```
tcpdump -i em1 tcp dst port 80 -d
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

  tcpdump使用`setsockopt`函数(实际在libpcap库中)的`SO_ATTACH_FILTER`操作，将过滤程序绑定到socket。

> Linux-2.2之后的版本，支持`SO_ATTACH_FILTER`

  `setsockopt`函数原型如下：

```
  #include <sys/types.h>
  #incldue <sys/socket.h>
  int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

  当setsockopt的optname参数指定为`SO_ATTACH_FILTER`时，optval参数类型为结构体`struct sock_fprog`，其成员`filter`指向过滤程序，每个`struct sock_filter`结构体代表一条指令。结构体定义于`linux/filter.h`，如下：

```
  struct sock_filter {
    __u16 code; /* 汇编指令 */
    __u8  jt;    /* 当指令执行结果为True时，跳转目的 */
    __u8  jf;    /* 当指令执行为False时，跳转目的 */
    __u32 k;    /* 指令的参数 */
  };

  struct sock_fprog {
    unsigned short len;
    struct sock_filter *filter;
  };
```

> 使用setsockopt()函数的例子，见[sock_attach](code/sock_attach.c)

#### BPF虚机

  过滤程序使用的是BPF虚机指令，这是一种中间代码，注入内核后，由内核的Interpreter解释执行或者由JIT翻译为Native指令。
     
  在最新的内核中，传统的BPF(cBPF)是基于eBPF实现的了。
