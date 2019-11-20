# 使用XDP(eXpress Data Path)

XDP(eXpress Data Path)是基于内核eBPF的高速包处理框架，其特点如下：

+ 在网卡驱动中加入HOOK点，HOOK点位于收包的早期(在中断之后，进入协议栈之前)
+ 在HOOK点执行用户空间注入的eBPF程序(`bpf_prog_run_xdp()`)，进行包处理，可以丢包，进入协议栈或通过`AF_XDP socket`进入用户空间 
+ `AF_XDP socket`是双向的(可以收包和发包)

----
1. 内核选项

```
CONFIG_BPF
CONFIG_BPF_SYSCALL
CONFIG_BPF_JIT_ALWAYS_ON

CONFIG_XDP_SOCKET: net/xdp.c

```

2. 接口驱动

在`include/linux/netdevice.h`中定义的`struct net_device_ops`:

```
struct net_device_ops {
  ...
  int (*ndo_bpf)(struct net_device *dev, struct netdev_bpf *bpf);
  int (*ndo_xdp_xmit)(struct net_device *dev, int n,
                      struct xdp_frame **xdp,
                      u32 flags);
};

结构体`struct netdev_bpf`定义于`linux/netdevice.h`，`struct xdp_frame`定义于`net/xdp.h`。

```


----
