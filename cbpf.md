# cBPF

cBPF就是传统的BPF(Berkeley Packet Filtering)，是用来捕获和过滤网络报文的，见[The BSD Packet Filter](bpf-usenix93.pdf)。

## cBPF简介

tcpdump的`-d`选项，可以将包过滤条件，翻译为cBPF汇编指令。

## cBPF在内核的实现

`SO_ATTACH_FILTER`

## cBPF的JIT及其实现
