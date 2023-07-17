//
// Created by paul on 25.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_MODULE_H
#define FRUITLANG_MODULE_H

#include "ast.h"
#include "../Typechecker.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace fruitlang {

    class Module : public ast {
    private:
        std::string name;
        std::vector<std::shared_ptr<ast>> contents;

    protected:
        uint64_t render_dot(std::ofstream &) override;
        llvm::Function *TopLVLCodegen([[maybe_unused]] Typechecker &) override;

    public:
        Module(std::string name, std::vector<std::shared_ptr<ast>> contents) : name(std::move(name)), contents(std::move(contents)){};
        ~Module() override = default;

        std::vector<Type> get_types([[maybe_unused]] Typechecker &) override;
    };
}// namespace fruitlang

#endif//FRUITLANG_MODULE_H
