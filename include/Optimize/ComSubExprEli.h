#ifndef SYSYF_COMSUBEXPRELI_H
#define SYSYF_COMSUBEXPRELI_H

#include "BasicBlock.h"
#include "Pass.h"
#include <map>
#include <memory>
#include <set>
#include "Instruction.h"
#include "internal_types.h"

namespace SysYF {
namespace IR {

struct cmp_expr{
    bool operator()(WeakPtr<Instruction> a, WeakPtr<Instruction> b) const {
        // TODO if a < b return true
        return false;
    }
};

/*****************************CommonSubExprElimination**************************************/
/***************************This class is based on SSA form*********************************/
class ComSubExprEli : public Pass {
public:
    explicit ComSubExprEli(WeakPtr<Module> m):Pass(m){}
    const std::string get_name() const override {return name;}
    void execute() override;
    void compute_local_gen(Ptr<Function> f);
    void compute_global_in_out(Ptr<Function> f);
    void compute_global_common_expr(Ptr<Function> f);
    /**
     * @brief init bb in/out/gen map with empty set
     * 
     * @param f 
     */
    void initial_map(Ptr<Function> f);
    static bool is_valid_expr(Ptr<Instruction> inst);
private:
    const std::string name = "ComSubExprEli";
    std::set<Ptr<Instruction>,cmp_expr> availableExprs;
    WeakPtrMap<BasicBlock, std::set<Ptr<Instruction>, cmp_expr>> bb_in;
    WeakPtrMap<BasicBlock, std::set<Ptr<Instruction>, cmp_expr>> bb_out;
    WeakPtrMap<BasicBlock, std::set<Ptr<Instruction>, cmp_expr>> bb_gen;
    bool rerun = false;
};

}
}

#endif // SYSYF_COMSUBEXPRELI_H
