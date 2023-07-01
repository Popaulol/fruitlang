//
// Created by paul on 25.06.23.
//

#include "expr.h"

namespace fruitlang {
    std::shared_ptr<Type> expr::get_type(Typechecker &typechecker) {
        if(type) return type;
        type = typecheck(typechecker);
        return type;
    }
}// namespace fruitlang