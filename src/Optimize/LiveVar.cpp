#include "LiveVar.h"
#include "BasicBlock.h"
#include "Value.h"
#include "internal_types.h"
#include <fstream>

#include <algorithm>
#include <memory>

namespace SysYF
{
namespace IR
{

void LiveVar::execute() {
    //  请不要修改该代码。在被评测时不要在中间的代码中重新调用set_print_name
    module.lock()->set_print_name();

    for (auto func : this->module.lock()->get_functions()) {
        if (func->get_basic_blocks().empty()) {
            continue;
        } else {
            func_ = func;
            
            /*you need to finish this function*/
        }
    }

    // 请不要修改该代码，在被评测时不要删除该代码
    dump();
    //
    return ;
}

void LiveVar::dump() {
    std::fstream f;
    f.open(lvdump, std::ios::out);
    for (auto &func: module.lock()->get_functions()) {
        for (auto &bb: func->get_basic_blocks()) {
            f << bb->get_name() << std::endl;
            auto &in = bb->get_live_in();
            auto &out = bb->get_live_out();
            auto sorted_in = sort_by_name(in);
            auto sorted_out = sort_by_name(out);
            f << "in:\n";
            for (auto in_v: sorted_in) {
                if(in_v.lock()->get_name() != "")
                {
                    f << in_v.lock()->get_name() << " ";
                }
            }
            f << "\n";
            f << "out:\n";
            for (auto out_v: sorted_out) {
                if(out_v.lock()->get_name() != ""){
                    f << out_v.lock()->get_name() << " ";
                }
            }
            f << "\n";
        }
    }
    f.close();
}


bool ValueCmp(WeakPtr<Value> a, WeakPtr<Value> b) {
    return a.lock()->get_name() < b.lock()->get_name();
}

WeakPtrVec<Value> sort_by_name(WeakPtrSet<Value> &val_set) {
    WeakPtrVec<Value> result;
    result.assign(val_set.begin(), val_set.end());
    std::sort(result.begin(), result.end(), ValueCmp);
    return result;
}

}
}
