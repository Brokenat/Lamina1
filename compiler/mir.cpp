//
// Created by meian on 2026/7/16.
//

#include "mir.hpp"
#include <sstream>

namespace lmx::mir {

std::string to_string(runtime::ValueKind type) {
    switch (type) {
    case runtime::ValueKind::Int:    return "int";
    case runtime::ValueKind::Fraction:   return "frac";
    case runtime::ValueKind::Bool:   return "bool";
    case runtime::ValueKind::Obj: return "obj";
    case runtime::ValueKind::Null: return "null";
    case runtime::ValueKind::C_Ptr:  return "c_ptr";
    }
    return "?";
}

std::string to_string(MirOp op) {
    switch (op) {
    case MirOp::IAdd: return "iadd";
    case MirOp::ISub: return "isub";
    case MirOp::IMul: return "imul";
    case MirOp::IDiv: return "idiv";
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

std::string to_string(const MirExpr &expr) {
    switch (expr.kind) {
        // return to_string(static_cast<const MirValue &>(expr));
    case MirExpr::Kind::Operate:
        return to_string(static_cast<const MirOperate &>(expr));
    case MirExpr::Kind::Value:
        break;
    }
    return "?";
}

std::string to_string(const MirValue &lit) {
    return to_string(lit.type) + " " + lit.data;
}

std::string to_string(const MirOperate &op) {
    std::ostringstream os;
    os << to_string(op.type) << " " << to_string(op.op);
    // for (const auto &arg : op.args) {
    //     os << " " << to_string(arg);
    // }
    return os.str();
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
