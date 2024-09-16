本项目提供SysYF语言的中间表示(IR)优化的编译器实践框架。SysYF语言是全国编译系统设计赛要求实现的SysY语言的扩展语言，SysYF IR兼容LLVM IR。该项目由中国科学技术大学张昱老师研究组设计和开发，旨在引导更多学生和社会人员学习和实践编译器相关知识，开展语言设计与实现、程序分析、编译优化等研发，让越来越多的人有能力运用程序语言基础提供可靠、高效的软件解决方案。

# 必做部分

必做部分为优化分析部分。你需要实现若干优化和分析。

## 编译命令的flag说明

本实验中新增了`-O2`命令行参数，使用`-O2`参数将开启全部优化（默认不开启）。例如：

```shell
./compiler -emit-ir -O2 test.sy -o test.ll
```

同时保留了`-emit-ast`参数用于从AST复原代码，`-check`参数用于静态检查。

若要开启单项优化，`-av`代表开启活跃变量分析。如果需要开启优化或者分析，则必须在命令行中搭配`-O`参数使用，否则无效。比如，若你想开启活跃变量分析或者其他分析优化遍，就需要使用如下命令：

```shell
./compiler -emit-ir -O -av test.sy -o test.ll
```

**注意：**以下两条命令产生的IR是不同的:

```shell
./compiler -emit-ir -O test.sy -o test.ll
```

```shell
./compiler -emit-ir test.sy -o test.ll
```

原因在于`-O`选项会开启Mem2Reg pass，IR会变成SSA格式。

有关flag详情，可以参见`src/main.cpp`。

## 分析与优化Pass说明

- Pass

  `Pass`类是所有分析与优化Pass的基类（比如Mem2Reg、DominateTree、ComSubExprEli等），定义在`include/Optimize/Pass.h`中。该基类中有两个纯虚函数需要子类重写：一个是`virtual void execute() = 0`，该函数是pass的总控函数，该函数会被调用去执行该pass；另一个是`virtual const std::string get_name() const = 0`，该函数用于获得该pass的名字。

  该基类中有一个成员变量`Module* module`，被子类继承。这里`module`含义表示一个编译单元，是一个源程序文件对应的中间表示。

  本实验中，你所实现的分析和优化pass均需继承自该类。你需要重写上述的`execute`函数和`get_name`函数。可以参考给出的`src/Optimize/Mem2Reg.cpp`和`src/Optimize/DominateTree.cpp`是如何继承Pass类并实现对应功能的。

- PassMgr

  `PassMgr`类负责管理Pass。定义在`include/Optimize/Pass.h`中。该类有两个私有成员变量:`module`和`pass_list`。`module`含义同上,`pass_list`为当前添加的所有pass的list。该类有两个公有的函数：`addPass`和`execute`。`addPass`函数用于向`pass_list`中添加pass，每次添加采用`push_back`的方式。`execute`函数用于顺序调用`pass_list`中pass的`execute`函数(即上述需要重写的`execute`函数)。`PassMgr`的使用实例可参见`src/main.cpp`。

  本实验中你无需修改`Pass`类和`PassMgr`类。如果有修改，请在报告中说明。

- Mem2Reg

  `Mem2Reg`用于将IR转换成为SSA形式的IR。以LLVM IR为例，在生成IR时，局部变量被生成为alloca/load/store的形式。用 alloca 指令来“声明”变量，得到一个指向该变量的指针，用 store 指令来把值存在变量里，用 load 指令来把值读出。LLVM 在 mem2reg 这个 pass 中，会识别出上述这种模式的 alloca，把它提升为 SSA value，在提升为 SSA value时会对应地消除 store 与 load，修改为 SSA 的 def-use/use-def 关系，并且在适当的位置安插 Phi 和 进行变量重命名。本次实验中，助教给出了Mem2Reg的一种实现(见`src/Optimize/Mem2Reg.cpp`)，在开启优化时会开启Mem2Reg，将IR转换为SSA形式的IR。因此本实验中的所有优化均基于SSA形式的IR。
  
## 必做关卡

必做部分共有四关（[第2关. 支配树](part1.md)、[第3关. Mem2Reg](part2.md)、[第4关. 活跃变量分析](part3.md)、[第5关. 检查pass](part4.md)）

## 使用Log输出程序信息

该部分文档见[logging.md](doc/logging.md)。
