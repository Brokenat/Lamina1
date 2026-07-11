//
// Created by meian on 2026/7/5.
//

#include "hir.hpp"

#include <ranges>

using namespace lmx;
using namespace lmx::hir;

Scope::Var HirContext::find_var(const std::string& name) noexcept {
    for (auto& i : scope_stack | std::views::reverse) {
        for (auto& j : i.vars) {
            if (j.first == name) return j;
        }
    }
    return {};
}

std::shared_ptr<Type> HirContext::inference_type(const std::shared_ptr<ExprNode> &type) noexcept {
    switch (type->kind) {
    case ASTKind::Literal: {
        const auto node = std::reinterpret_pointer_cast<LiteralNode>(type);
        switch (node->kind) {
        case LiteralNode::Kind::Integer: {
            return std::make_shared<BasicType>(runtime::ValueKind::Int);
        }
        case LiteralNode::Kind::Float: {
            return std::make_shared<BasicType>(runtime::ValueKind::Fraction);
        }
        case LiteralNode::Kind::String: {
            return std::make_shared<StringType>();
        }
        case LiteralNode::Kind::Boolean: {
            return std::make_shared<BasicType>(runtime::ValueKind::Bool);
        }
        }
    }
    case ASTKind::Identifier: {
        const auto node = std::reinterpret_pointer_cast<IdentifierNode>(type);
        if (node->type && node->type->kind != TypeKind::Unknown) return node->type;
        return find_var(node->id).second;
    }
    case ASTKind::Unary: {
        const auto node = std::reinterpret_pointer_cast<UnaryNode>(type);
        if (const auto t = inference_type(node->expr);
            t &&
            t->kind == TypeKind::Basic
            ) {
            if (const auto t2 = std::reinterpret_pointer_cast<BasicType>(t)->type;
                t2 == runtime::ValueKind::Int || t2 == runtime::ValueKind::Fraction) {

                return std::make_shared<BasicType>(t2);
            }
        }
    }
    case ASTKind::Binary: {
        const auto node = std::reinterpret_pointer_cast<BinaryNode>(type);
        auto left_ty = inference_type(node->lhs);
        const auto right_ty = inference_type(node->rhs);
        if (left_ty->equals(right_ty.get())) return left_ty;
    }
    case ASTKind::Block: {
        const auto node = std::reinterpret_pointer_cast<BlockExprNode>(type);
        if (node->stmts.back()->kind == ASTKind::TailReturn) {
            const auto tail_ret = std::reinterpret_pointer_cast<TailReturnNode>(node->stmts.back());
            if (tail_ret->expr && tail_ret->expr->type->kind != TypeKind::Unknown) return tail_ret->expr->type;
            return inference_type(tail_ret->expr);
        } //否则就是Block没有返回值
        break;
    }
    case ASTKind::SuffixParen: {
        const auto node = std::reinterpret_pointer_cast<SuffixParenNode>(type);
    }
    case ASTKind::SuffixBracket: {

    }
    case ASTKind::IfExpr: {
        const auto node = std::reinterpret_pointer_cast<IfExprNode>(type);

        return inference_type(node->then);
    }
    default: std::unreachable();
    }
    return nullptr;
}


void HirContext::reset() noexcept {
    scope_stack.clear();
}

void HirContext::check_module(Module *mod) noexcept {
    reset();
    
}
