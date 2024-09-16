## B4 检查Pass

由于对框架代码不熟悉、对优化算法理解不深或者在编码实现时的疏忽，我们很容易在实现了某个优化Pass之后在不经意间就对原本编译器中数据结构的一些约定进行了破坏（包括如LLVM IR中不同基本块之间的前驱后继关系，指令的操作数必须有定值才能使用，基本块必须以br、ret等指令结尾，def-use链等）。虽然可以通过打印LLVM IR并执行来确定某个Pass的正确性，并且这在很大程度上可以保证经过了该Pass优化生成的IR执行是正确的，但是这只能说明当前打印得到的IR是没有问题的，不能说明在编译器中仍然保留的IR Module是符合约定的。经过一个Pass之后的坏的IR Module可能会由于不满足约定而毒害下一个原本正确的优化遍导致出错。因此必要时我们可以在代码变换优化Pass之后添加一个**检查Pass**，来验证经过一个Pass变换之后的IR Module仍然满足一些良好的基本性质。

- **任务4-1**

  - **B4-1**. 基于你对LLVM IR Module的理解，在[include/Optimize/Check.h](include/Optimize/Check.h)和[src/Optimize/Check.cpp](src/Optimize/Check.cpp)中编写相应的约定验证代码，并在[main.cpp](src/main.cpp)或其他合适的位置插入相应的代码来利用你所写的检查器检查是否满足约定。将你检查的内容记录在报告[doc/reports/B4.md](doc/reports/B4.md)中，并说明检查理由。将检查器插入的代码位置记录在报告中，说明理由。
