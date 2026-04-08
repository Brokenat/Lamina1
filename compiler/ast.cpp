//
// Created by meian on 2026/4/3.
//

#include "ast.hpp"

#include <ranges>
#include <utility>
using namespace lmx;

Type::~Type() = default;

BasicType::~BasicType() = default;

ArrayType::~ArrayType() = default;

bool BasicType::equals(Type *other) const noexcept {
    if (!other) return false;
    if (other->kind != this->kind) return false;
    const auto *o = static_cast<BasicType *>(other);
    if (o->type != this->type) return false;
    return true;
}

bool ArrayType::equals(Type *other) const noexcept {
    if (!other) return false;
    if (other->kind != this->kind) return false;
    const auto *o = static_cast<ArrayType *>(other);
    if (this->type->equals(o->type.get())) return false;
    if (this->len != o->len) return false;
    return true;
}

ASTNode::ASTNode(const ASTKind kind, const size_t line, const size_t col) noexcept
    : kind(kind), line(line), col(col) {
}

ExprNode::ExprNode(const ASTKind kind, const size_t line, const size_t col) noexcept
    : ASTNode(kind, line, col) {
}

StmtNode::StmtNode(const ASTKind kind, const size_t line, const size_t col) noexcept
    : ASTNode(kind, line, col) {
}

ExprStmtNode::ExprStmtNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> expr) noexcept
    : StmtNode(ASTKind::ExprStmt, line, col), expr(std::move(expr)) {
}

ExprsNode::ExprsNode(const size_t line, const size_t col, std::vector<std::shared_ptr<ExprNode> > exprs) noexcept
    : ExprNode(ASTKind::Exprs, line, col), exprs(std::move(exprs)) {
}

LiteralNode::LiteralNode(const size_t line, const size_t col, std::string val, const Kind kind) noexcept
    : ExprNode(ASTKind::Literal, line, col), val(std::move(val)), kind(kind) {
}

IdentifierNode::IdentifierNode(const size_t line, const size_t col, std::string id) noexcept
    : ExprNode(ASTKind::Identifier, line, col), id(std::move(id)) {
}

SuffixParenNode::SuffixParenNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> expr,
                                 std::shared_ptr<ExprsNode> suffix) noexcept
    : ExprNode(ASTKind::SuffixParen, line, col), expr(std::move(expr)), suffix(std::move(suffix)) {
}

SuffixBracketNode::SuffixBracketNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> expr,
                                     std::shared_ptr<ExprNode> suffix) noexcept
    : ExprNode(ASTKind::SuffixBracket, line, col), expr(std::move(expr)), suffix(std::move(suffix)) {
}

UnaryNode::UnaryNode(const size_t line, const size_t col, std::string op, std::shared_ptr<ExprNode> expr) noexcept
    : ExprNode(ASTKind::Unary, line, col), op(std::move(op)), expr(std::move(expr)) {
}

BinaryNode::BinaryNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> lhs, std::string op,
                       std::shared_ptr<ExprNode> rhs) noexcept
    : ExprNode(ASTKind::Binary, line, col), op(std::move(op)), lhs(std::move(lhs)), rhs(std::move(rhs)) {
}

ReturnNode::ReturnNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> expr) noexcept
    : StmtNode(ASTKind::Return, line, col), expr(std::move(expr)) {
}

ReturnNode::ReturnNode(const size_t line, const size_t col, const std::shared_ptr<ExprStmtNode> &s) noexcept
    : StmtNode(ASTKind::Return, line, col), expr(s->expr) {
}

TailReturnNode::TailReturnNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> expr) noexcept
    : StmtNode(ASTKind::TailReturn, line, col), expr(std::move(expr)) {}

TailReturnNode::TailReturnNode(const size_t line, const size_t col, const std::shared_ptr<ExprStmtNode> &s) noexcept
    : StmtNode(ASTKind::TailReturn, line, col), expr(s->expr) {}

BlockExprNode::BlockExprNode(const size_t line, const size_t col,
                             std::vector<std::shared_ptr<StmtNode> > stmts) noexcept
    : ExprNode(ASTKind::Block, line, col), stmts(std::move(stmts)) {
}

BreakStmtNode::BreakStmtNode(const size_t line, const size_t col) noexcept
    : StmtNode(ASTKind::BreakStmt, line, col) {}

ParamsDeclNode::ParamsDeclNode(const size_t line, const size_t col,
                               std::map<std::shared_ptr<ExprStmtNode>, std::shared_ptr<Type> > stmts) noexcept
    : StmtNode(ASTKind::ParamsDeclNode, line, col), stmts(std::move(stmts)) {
}

FuncImplNode::FuncImplNode(const size_t line, const size_t col,
    std::shared_ptr<StmtNode> func_id,
    std::shared_ptr<ParamsDeclNode> params,
    std::shared_ptr<Type> return_type,
    std::shared_ptr<BlockExprNode> block) noexcept
    : StmtNode(ASTKind::FuncImpl, line, col), func_id(std::move(func_id)),params(std::move(params)),return_type(std::move(return_type)), block(std::move(block)) {}

IfExprNode::IfExprNode(const size_t line, const size_t col, std::shared_ptr<ExprNode> cond, std::shared_ptr<ExprNode> then, std::shared_ptr<ASTNode> els) noexcept
    : ExprNode(ASTKind::IfExpr, line, col), cond(std::move(cond)), then(std::move(then)), els(std::move(els)) {}

VarDeclNode::VarDeclNode(const size_t line, const size_t col, std::shared_ptr<ExprStmtNode> id, std::shared_ptr<Type> type, const bool is_mutable) noexcept
    : StmtNode(ASTKind::VarDecl, line, col), id(std::move(id)), type(std::move(type)), is_mutable(is_mutable) {}
