#pragma once
#include "ast.hpp"

namespace lmx::hir {
/*
 * Lamina Compiler HIR
 * 这个阶段其实并不产生新东西，一开始想产生，后来发现必要性不大，
 * 现在它用来初步验证语法树正确性，填补空缺的数据类型
 * 完成后，产生一份符号表
 */

using HirNode = ASTNode;
struct Scope {
    using Var = std::pair<std::string, std::shared_ptr<Type>>;
    std::string name;
    std::vector<Var> vars;
};

class HirContext {
    std::vector<Scope> scope_stack;

    Scope::Var find_var(const std::string& name) noexcept;
public:

    void check_module(Module *mod) noexcept;
    void reset() noexcept;
    std::shared_ptr<Type> inference_type(const std::shared_ptr<ExprNode>& type) noexcept;
};

}
