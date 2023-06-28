//
// Created by paul on 28.06.23.
//

#include "access.h"
#include "../codegen.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t access::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"Access: " << name << "\"];\n";
        return own_id;
    }
    llvm::Value *access::codegen() {
        llvm::Value *V = NamedValues[name];
        if(!V) CodegenError("Unknown variable name");
        return V;
    }
}// namespace fruitlang