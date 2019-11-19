# 使用XDP(eXpress Data Path)


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
