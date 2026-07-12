//
// Created by meian on 2026/7/5.
//

#include "hir.hpp"

#include <functional>
#include <ranges>

#include "error.hpp"

using namespace lmx;
using namespace lmx::hir;

std::optional<Scope::Var> HirContext::find_var(const std::string &name) noexcept {
    for (auto& i : scope_stack | std::views::reverse) {
        for (auto& j : i.vars) {
            if (j.name == name) return j;
        }
    }
    return std::nullopt;
}

std::shared_ptr<Type> HirContext::inference_type(ExprNode* type) noexcept {
    switch (type->kind) {
    case ASTKind::Literal: {
        const auto node = reinterpret_cast<LiteralNode*>(type);
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
        const auto node = reinterpret_cast<IdentifierNode*>(type);
        if (node->type && node->type->kind != TypeKind::Unknown) return node->type;
        if (find_var(node->id).has_value())return find_var(node->id)->type;
        break;
    }
    case ASTKind::Unary: {
        const auto node = reinterpret_cast<UnaryNode*>(type);
        if (const auto t = inference_type(node->expr.get());
            t &&
            t->kind == TypeKind::Basic
            ) {
            if (const auto t2 = std::reinterpret_pointer_cast<BasicType>(t)->type;
                t2 == runtime::ValueKind::Int || t2 == runtime::ValueKind::Fraction) {

                return std::make_shared<BasicType>(t2);
            }
        }
        break;
    }
    case ASTKind::Binary: {
        const auto node = reinterpret_cast<BinaryNode*>(type);
        auto left_ty = inference_type(node->lhs.get());
        const auto right_ty = inference_type(node->rhs.get());
        if (left_ty->equals(right_ty.get())) return left_ty;
        break;
    }
    case ASTKind::Block: {
        const auto node = reinterpret_cast<BlockExprNode*>(type);
        if (node->stmts.back()->kind == ASTKind::TailReturn) {
            const auto tail_ret = std::reinterpret_pointer_cast<TailReturnNode>(node->stmts.back());
            if (tail_ret->expr &&
                !Type::is_null_type(tail_ret->expr->type.get()) &&
                tail_ret->expr->type->kind != TypeKind::Unknown

                ) return tail_ret->expr->type;
            return inference_type(tail_ret->expr.get());
        } //否则就是Block没有返回值
        break;
    }
    case ASTKind::SuffixParen: {
        const auto node = reinterpret_cast<SuffixParenNode*>(type);
        break;
    }
    case ASTKind::SuffixBracket: {
        const auto node = reinterpret_cast<SuffixBracketNode*>(type);
        const auto left_t = inference_type(node->expr.get());
        if (left_t->kind == TypeKind::Array) {
            return std::reinterpret_pointer_cast<ArrayType>(left_t)->type;
        }
        break;
    }
    case ASTKind::IfExpr: {
        const auto node = reinterpret_cast<IfExprNode*>(type);

        return inference_type(node->then.get());
    }
    default: std::unreachable();
    }
    return std::make_shared<NoneType>();
}


void HirContext::reset() noexcept {
    scope_stack.clear();
}

void HirContext::check_module(const Module *mod) noexcept {
    reset();
    for (auto& node : mod->decls) {
        check_stmt(node.get());
    }
}
void HirContext::check_expr(ExprNode *expr) noexcept {
    switch (expr->kind) {
    case ASTKind::Literal: {
        expr->type = inference_type(expr);
        break;
    }
    case ASTKind::Identifier: {
        auto* node = reinterpret_cast<IdentifierNode*>(expr);
        auto re = find_var(node->id);
        if (!re.has_value())
            throw_error(ErrorType::Analysis, "undefined var `" + node->id + "`", node->line, node->col);
        node->type = re->type;
        break;
    }
    case ASTKind::Unary: {
        auto* node = reinterpret_cast<UnaryNode*>(expr);
        const auto type = inference_type(node->expr.get());
        if (type->kind != TypeKind::Basic) goto unary_type_mismatch;
        if (const auto t2 = std::reinterpret_pointer_cast<BasicType>(type);
            t2->type != runtime::ValueKind::Int && t2->type != runtime::ValueKind::Fraction) goto unary_type_mismatch;
        node->type = type;
        break;
        unary_type_mismatch:
        throw_error(ErrorType::Analysis, "unary`-` cannot applied to this type", expr->line, expr->col);
        break;
    }
    case ASTKind::Binary: {
        auto* node = reinterpret_cast<BinaryNode*>(expr);
        check_expr(node->lhs.get());
        check_expr(node->rhs.get());
        const auto lty = inference_type(node->lhs.get());
        const auto rty = inference_type(node->rhs.get());
        node->lhs->type = lty;
        node->rhs->type = rty;
        if (!lty->equals(rty.get())) throw_error(ErrorType::Analysis, "binary operation type mismatch", expr->line, expr->col);

        if (lty->kind != TypeKind::Basic) goto binary_type_mismatch;
        if (const auto t2 = std::reinterpret_pointer_cast<BasicType>(lty);
            t2->type != runtime::ValueKind::Int && t2->type != runtime::ValueKind::Fraction) goto binary_type_mismatch;

        node->type = lty;
        break;
        binary_type_mismatch:
        throw_error(ErrorType::Analysis, "binary operation cannot applied to this type", expr->line, expr->col);
        break;
    }
    case ASTKind::Block: {
        auto node = reinterpret_cast<BlockExprNode*>(expr);
        for (auto& s : node->stmts) check_stmt(s.get());
        expr->type = inference_type(expr);
        break;
    }
    case ASTKind::SuffixParen: {
        //auto func = find_var();
        break;
    }
    case ASTKind::SuffixBracket: {

        break;
    }
    case ASTKind::IfExpr: {
        break;
    }
    default: std::unreachable();
    }
}

void HirContext::check_stmt(StmtNode* stmt) noexcept {
    switch (stmt->kind) {
    case ASTKind::ExprStmt: {
        const auto* node = reinterpret_cast<ExprStmtNode*>(stmt);
        check_expr(node->expr.get());
        break;
    }

        break;
    case ASTKind::Exprs:
        break;
    case ASTKind::ParamsDeclNode:
        break;
    case ASTKind::FuncImpl: {
        auto* node = reinterpret_cast<FuncImplNode*>(stmt);

        Scope scope;
        for (const auto& [name, ty] : node->params->stmts) {
            scope.vars.emplace_back(name, ty, true);
        }
        scope.name = "";
        scope.return_type = node->return_type;
        scope_stack.push_back(scope);

        check_expr(node->block.get());
        if (Type::is_null_type(node->return_type.get())) node->return_type = node->block->type;
        scope_stack.pop_back();
        break;
    }
    case ASTKind::Return: {
        auto node = reinterpret_cast<ReturnNode*>(stmt);
        check_expr(node->expr.get());
        node->expr->type = inference_type(node->expr.get());
        break;
    }
    case ASTKind::TailReturn: {
        auto node = reinterpret_cast<TailReturnNode*>(stmt);
        check_expr(node->expr.get());
        // node->expr->type = inference_type(node->expr.get());
        break;
    }
    case ASTKind::VarDecl: {

        break;
    }
    case ASTKind::GlobalVarDecl:
        break;
    case ASTKind::BreakStmt: {
        break;
    }
    default: std::unreachable();
    }
}

