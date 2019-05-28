# eBPF介绍

  eBPF虚机是一个RISC CPU，具有11个64-bit Register, 1个PC(Program Counter)和512Byte的Stack。寄存器如下：

  | 寄存器 | 说明 |
  |--------|------|
  | r0     | 存放返回值             |
  | r1-r5  | 函数调用时，存放参数   |
  | r6-r9  |        |
  | r10    | 只读，栈指针(Stack Pointer)    |


