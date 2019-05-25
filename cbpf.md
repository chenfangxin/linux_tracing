# cBPF

cBPF就是传统的BPF(Berkeley Packet Filtering)，是用来捕获和过滤网络报文的，见[The BSD Packet Filter](bpf-usenix93.pdf)。

Linux内核中，BPF的发展历程：
  * 从Linux-2.5开始支持BPF
  * 在2011年，Linux内核实现BPF JIT(By Eric Dumazet)
  * 在2014年，Linux内核重新实现了BPF JIT，称为eBPF(By Alexei Starovoitov)

## cBPF简介

tcpdump的`-d`选项，可以将包过滤条件，翻译为cBPF汇编指令。例如下面的指令，捕获所有目的端口为80的tcp包：

```
tcpdump -d "tcp dst port 80"

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

## cBPF在内核的实现


`SO_ATTACH_FILTER`

## cBPF的JIT及其实现

