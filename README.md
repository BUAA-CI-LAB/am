# The Abstract Machine (AM) for LoongArch

## 介绍

该项目旨在便捷的创建 la32r 架构的裸机 C 程序及汇编程序。

目录组织：

* `am/`：AM头文件、每个体系结构分别实现的AM代码
* `libs/`：建立在AM上、体系结构无关的运行库，如软件模拟浮点数、基础 libc 功能等
* `apps/`：一些 benchmark，如 coremark，dhrystone，linux
* `tests/`: 用来测试 CPU/SoC 实现的测试程序

## 编译程序
设置环境变量：环境变量`AM_HOME`需要设置为 **am 项目的根目录的绝对路径**，如 `/home/am`。
* 设置 la32r 编译器与库的环境变量：将 `loongarch32r-linux-gnusf-2022-05-20` 与 `system_newlib` 放在一个文件夹 `la32r-toolchains` 下，并将 `$LA32RTC_HOME` 环境变量设置为 `la32r-toolchains` 目录。

```
├── la32r-toolchains
│   ├── loongarch32r-linux-gnusf-2022-05-20
│   └── system_newlib
```

编译时，首先确保环境变量是否正确设置，然后执行`make ARCH=体系结构-平台`编译。例如`make ARCH=la32r-eula` 编译生成可在 chiplab 及 difftest 仿真环境中运行的裸机程序。

* 注意：在 `$AM_HOME` 下 `make` 只会生成 am 的 archive 文件，在 ，`tests` 或 `apps` 下文件夹中 `make` 才会生成对应程序的 elf，bin 及反汇编文件

## 创建多源文件程序

程序用C/C++语言编写，除AM之外无法调用其他库函数（但可以引用`stdarg.h`, `limits.h`等包含体系结构相关数据定义的头文件）。

为此你需要在应用程序项目的根目录添加一个Makefile（以 `apps/coremark` 为例）：

```
NAME = coremark
SRCS = $(shell find -L ./src/ -name "*.c")
include $(AM_HOME)/Makefile.app
```

一些注意事项：

* `NAME`定义了应用的名字。编译后会在`build/`目录里出现以此命名的应用程序
* `SRCS`指定了编译应用所需的源文件。可以放在应用目录中的任意位置

* 应用目录下的`include/`目录会被添加到编译的`-I`选项中

## 创建单源文件程序

Makefile 编写及文件夹结构请参考 `tests/cputest`

## chiplab 仿真

chiplab 框架需要参考[这个 commit](http://gitlab.ci-lab.net/ghy/chiplab/-/commit/eeae39f9e39cc435dba749ccd41e972a7fc769a4) 修改。然后在 `$CHIPLAB_HOME/sims/verilator/run_prog` 目录下执行 `./configure.sh --run ext_prog`，`make EXTPROG=$AM_HOME/apps/coremark/build/coremark-la32r-eula.vlog` 就可以执行 am 编译出的 coremark 程序。

## How It Works

一般情况下所有程序共用 `ARCH` 指定的编译命令，链接脚本及初始化代码。以 `ARCH=la32r-eula` 为例，cpu 复位后指令流为：
```
am/src/eula/isa/la32r/boot/start.S        // 初始化栈指针
am/src/eula/isa/la32r/trm.c : _trm_init   // 初始化外设，初始化缓存，建立地址空间映射等
执行对应程序 main 函数
返回 _trm_init 执行 halt 函数结束仿真
```
* 目前在 `halt` 函数中尝试使用 `syscall 0x11` 指令结束程序运行。运行 AM 程序的目标平台应当支持此种方法（la32r-nemu 支持，因此使用 la32r-nemu 协同仿真的 difftest 框架及 chiplab 框架均支持此方法）。

## MISC

* 本项目移植自 [nexus-am](https://github.com/OpenXiangShan/nexus-am)
* 有关 Abstract-Machine 的 [更多信息](https://jyywiki.cn/OS/AbstractMachine/index.html)