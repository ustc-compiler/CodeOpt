#ifndef SYSYF_LIVEVAR_H
#define SYSYF_LIVEVAR_H

#include "Pass.h"
#include "Module.h"

namespace SysYF{
namespace IR{

class LiveVar : public Pass
{
public:
    LiveVar(Ptr<Module> m) : Pass(m) {}
    void execute() final;
    void get_def_use();
    void get_live_in_live_out();
    const std::string get_name() const override {return name;}
    void dump();
private:
    Ptr<Function> func_;
    const std::string name = "LiveVar";
};

bool ValueCmp(Ptr<Value> a, Ptr<Value> b);
PtrVec<Value> sort_by_name(PtrSet<Value> &val_set);
const std::string lvdump = "live_var.out";

}
}

#endif  // SYSYF_LIVEVAR_H
