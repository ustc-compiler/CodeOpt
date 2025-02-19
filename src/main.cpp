#include <iostream>
#include "Check.h"
#include "ComSubExprEli.h"
#include "IRBuilder.h"
#include "SysYFDriver.h"
#include "SyntaxTreePrinter.h"
#include "ErrorReporter.h"
#include "Pass.h"
#include "DominateTree.h"
#include "Mem2Reg.h"
#include "LiveVar.h"
#include "SyntaxTreeChecker.h"


void print_help(const std::string& exe_name) {
  std::cout << "Usage: " << exe_name
            << " [ -h | --help ] [ -p | --trace_parsing ] [ -s | --trace_scanning ] [ -emit-ast ] [ -check ]"
            << " [ -emit-ir ] [ -O2 ] [ -O ] [ -lv ] [ -cse ] [ -o <output-file> ]"
            << " <input-file>"
            << std::endl;
}

int main(int argc, char *argv[])
{
    using namespace SysYF;

    SysYFDriver driver;
    SyntaxTree::SyntaxTreePrinter printer;
    ErrorReporter reporter(std::cerr);
    SyntaxTree::SyntaxTreeChecker checker(reporter);
    auto builder = IR::IRBuilder::create();

    bool print_ast = false;
    bool emit_ir = false;
    bool check = false;
    bool optimize_all = false;
    bool optimize = false;

    bool lv = false;
    bool cse = false;

    std::string filename = "-";
    std::string output_llvm_file = "-";
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string("-h") || argv[i] == std::string("--help")) {
            print_help(argv[0]);
            return 0;
        }
        else if (argv[i] == std::string("-p") || argv[i] == std::string("--trace_parsing")) {
            driver.trace_parsing = true;
        }
        else if (argv[i] == std::string("-s") || argv[i] == std::string("--trace_scanning")){
            driver.trace_scanning = true;
        }
        else if (argv[i] == std::string("-emit-ast")) {
            print_ast = true;
        }
        else if (argv[i] == std::string("-emit-ir")){
            emit_ir = true;
        }
        else if (argv[i] == std::string("-o")){
            output_llvm_file = argv[++i];
        }
        else if (argv[i] == std::string("-check")){
            check = true;
        }
        else if (argv[i] == std::string("-O2")){
            optimize_all = true;
            optimize = true;
        }
        else if (argv[i] == std::string("-O")){
            optimize = true;
        }
        else if(argv[i] == std::string("-lv")){
            optimize = true;
            lv = true;
        }
        else if(argv[i] == std::string("-cse")){
            optimize = true;
            cse = true;
        }
        //  ...
        else {
            filename = argv[i];
        }
    }
    auto root = driver.parse(filename);
    if (print_ast)
        root->accept(printer);
    if (check)
        root->accept(checker);
    if (emit_ir) {
        root->accept(*builder);
        auto m = builder->getModule();
        m->set_file_name(filename);
        m->set_print_name();
        if(optimize){
            IR::PassMgr passmgr(m);
            passmgr.addPass<IR::DominateTree>();
            passmgr.addPass<IR::Mem2Reg>();
            if(optimize_all){
                passmgr.addPass<IR::LiveVar>();
                passmgr.addPass<IR::ComSubExprEli>();
                passmgr.addPass<IR::Check>();
                //  ...
            }
            else {
                if(lv){
                    passmgr.addPass<IR::LiveVar>();
                }
                if(cse){
                    passmgr.addPass<IR::ComSubExprEli>();
                    passmgr.addPass<IR::Check>();
                }
                //  ...
            }
            passmgr.execute();
            m->set_print_name();
        }
        auto IR = m->print();
        if(output_llvm_file == "-"){
            std::cout << IR;
        }
        else {
            std::ofstream output_stream;
            output_stream.open(output_llvm_file, std::ios::out);
            output_stream << IR;
            output_stream.close();
        }
    }
    return 0;
}
