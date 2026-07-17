//
// Created by meian on 2026/7/16.
//

#include "mir.hpp"
#include <sstream>

namespace lmx::mir {

std::string to_string(MirType type) {
    switch (type) {
    case MirType::Int:    return "int";
    case MirType::Frac:   return "frac";
    case MirType::Bool:   return "bool";
    case MirType::String: return "string";
    }
    return "?";
}

std::string to_string(MirOp op) {
    switch (op) {
    case MirOp::Add: return "add";
    case MirOp::Sub: return "sub";
    case MirOp::Mul: return "mul";
    case MirOp::Div: return "div";
    case MirOp::Le:  return "le";
    case MirOp::Lt:  return "lt";
    case MirOp::Ge:  return "ge";
    case MirOp::Gt:  return "gt";
    case MirOp::Eq:  return "eq";
    case MirOp::Ne:  return "ne";
    case MirOp::And: return "and";
    case MirOp::Or:  return "or";
    }
    return "?";
}

std::string to_string(const MirRef &ref) {
    std::ostringstream os;
    os << to_string(ref.type) << " " << (ref.is_temp ? "%" : "$") << ref.name;
    return os.str();
}

std::string to_string(const MirExpr &expr) {
    switch (expr.kind) {
    case MirExpr::Kind::Literal:
        return to_string(static_cast<const MirLiteral &>(expr));
    case MirExpr::Kind::Operate:
        return to_string(static_cast<const MirOperate &>(expr));
    case MirExpr::Kind::Ref:
        return to_string(static_cast<const MirRefExpr &>(expr));
    }
    return "?";
}

std::string to_string(const MirLiteral &lit) {
    return to_string(lit.type) + " " + lit.data;
}

std::string to_string(const MirOperate &op) {
    std::ostringstream os;
    os << to_string(op.type) << " " << to_string(op.op);
    for (const auto &arg : op.args) {
        os << " " << to_string(arg);
    }
    return os.str();
}

std::string to_string(const MirRefExpr &ref) {
    return to_string(ref.ref);
}

std::string to_string(const MirStmt &stmt) {
    std::ostringstream os;
    switch (stmt.kind) {
    case MirStmt::Kind::Expr: {
        auto &s = static_cast<const MirExprStmt &>(stmt);
        os << to_string(*s.expr);
        break;
    }
    case MirStmt::Kind::TmpAssign: {
        auto &s = static_cast<const MirTmpAssignStmt &>(stmt);
        os << "%" << s.name << " = " << to_string(*s.expr);
        break;
    }
    case MirStmt::Kind::Assign: {
        auto &s = static_cast<const MirAssignStmt &>(stmt);
        os << "$" << s.name << " = " << to_string(*s.expr) << ";";
        break;
    }
    }
    return os.str();
}

std::string to_string(const MirProgram &prog) {
    std::ostringstream os;
    for (const auto &stmt : prog) {
        os << to_string(*stmt) << "\n";
    }
    return os.str();
}

}
