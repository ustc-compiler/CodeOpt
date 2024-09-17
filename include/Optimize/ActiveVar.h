#ifndef SYSYF_ACTIVEVAR_H
#define SYSYF_ACTIVEVAR_H

#include "Pass.h"
#include "Module.h"

namespace SysYF{
namespace IR{

class ActiveVar : public Pass
{
public:
    ActiveVar(Ptr<Module> m) : Pass(m) {}
    void execute() final;
    void get_def_use();
    void get_live_in_live_out();
    const std::string get_name() const override {return name;}
    void dump();
private:
    Ptr<Function> func_;
    const std::string name = "ActiveVar";
};

bool ValueCmp(Ptr<Value> a, Ptr<Value> b);
PtrVec<Value> sort_by_name(PtrSet<Value> &val_set);
const std::string avdump = "active_var.out";

}
}

#endif  // SYSYF_ACTIVEVAR_H
