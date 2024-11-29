#include "Pass.h"
#include "ComSubExprEli.h"
#include <set>
#include <algorithm>

namespace SysYF {
namespace IR {

void ComSubExprEli::execute() {
}

bool ComSubExprEli::is_valid_expr(Ptr<Instruction> inst) {
    return !(
        inst->is_void() // ret, br, store, void call
        || inst->is_phi()
        || inst->is_alloca()
        || inst->is_load()
        || inst->is_call()
        || inst->is_cmp()
        || inst->is_fcmp()
    );
}

}
}
