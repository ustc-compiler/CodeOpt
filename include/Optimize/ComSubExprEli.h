#ifndef SYSYF_COMSUBEXPRELI_H
#define SYSYF_COMSUBEXPRELI_H

#include "Pass.h"
#include <map>
#include <set>
#include "Instruction.h"
#include "internal_types.h"

namespace SysYF {
namespace IR {

/*****************************CommonSubExprElimination**************************************/
/***************************This class is based on SSA form*********************************/
class ComSubExprEli : public Pass {
public:
    explicit ComSubExprEli(Ptr<Module> m):Pass(m){}
    const std::string get_name() const override {return name;}
    void execute() override;
    static bool is_valid_expr(Ptr<Instruction> inst);
private:
    const std::string name = "ComSubExprEli";
    bool rerun = false;
};

}
}

#endif // SYSYF_COMSUBEXPRELI_H