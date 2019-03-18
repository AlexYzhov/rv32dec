# RV32指令分析器

本程序可对输入的16进制RV32指令数据(e.g 0x00000013, nop指令)进行分析，拆解指令中的各项操作数(opcode)、操作码(funct)、立即数(imm)和寄存器索引(idx)，同时描述该指令的基本功能，便于学习和分析RV32指令。

---

## 工程组成

文件夹在初始状态下包含：

1. ***/.vscode***               文件夹
2. ***/src***                   文件夹
3. ***/src/rv32decoder.c***     主程序代码
4. ***/src/rv32decoder.h***     主程序宏定义文件
5. ***/src/rv32i_def.h***       rv32i子集宏定义文件
6. ***/src/console_color.h***   控制台ANSI Color宏定义文件
7. ***rv32decoder.exe***        win64可执行程序
8. ***rv32decoder***            linux x64可执行程序

---

## 准备工作

1. 根据平台编译对应的可执行文件
2. 准备好可执行文件的环境变量

---

## 使用说明

1. 打开终端，执行 **rv32decoder** , 将需要被分析的RV32指令序列, 以16进制的形式, 作为启动参数添加在调用命令末端.
2. 程序按照顺序返回各条指令的意义和拆解项. 若终端支持ANSI-Color, 则会有不同的颜色高亮各种类型的数据, 以示区分. 若终端不支持ANSI-Color, 则无高亮. 源代码 ***/src/rv32decoder.c*** 中默认对linux平台生成带有ANSI-Color指令的输出, windows平台默认终端(如powershell)因为不支持ANSI-Color, 所以默认不使用ANSI-Color, 输出数据无高亮.

e.g:
> user@pc:~$ rv32decoder 0x00000013 0xc0010113 0x001281b3 0x00008063 0xffdff0ef

![image](https://github.com/AlexYzhov/rv32dec/blob/master/example.png)

