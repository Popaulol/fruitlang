//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_MODULE_H
#define FRUITLANG_MODULE_H

#include "ast.h"
#include "definition.h"
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
        llvm::Function *TopLVLCodegen() override;

    public:
        Module(std::string name, std::vector<std::shared_ptr<ast>> contents) : name(std::move(name)), contents(std::move(contents)){};
        ~Module() override = default;
    };

}// namespace fruitlang

#endif//FRUITLANG_MODULE_H
