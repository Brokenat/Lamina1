//
// Created by meian on 2026/7/16.
//

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "ast.hpp"

namespace lmx::mir {

using MirType = Type;
using MirBasicType = BasicType;
using MirStringType = StringType;
using MirArrayType = ArrayType;
using MirFunctionType = FunctionType;
using MirNoneType = NoneType;

struct MirVar {
    std::shared_ptr<MirType> type;
    bool is_temp;
    std::string name;
};

enum class MirOp {
    IAdd, ISub, IMul, IDiv, IMod, IPow,
    FAdd, FSub, FMul, FDiv, FMod, FPow,
    Le, Lt, Ge, Gt, Eq, Ne,
    And, Or,
    IfTrue, IfFalse, Goto,

    Call, Ret,
};

struct MirNode {
    virtual ~MirNode() = default;
};

struct MirExpr : MirNode {
    enum class Kind { Value, Operate };
    Kind kind;
    runtime::ValueKind type;
};

struct MirValue : MirExpr {
    runtime::ValueKind type;
    std::string data;
};

struct MirOperate : MirExpr {
    MirOp op;

    explicit MirOperate(const MirOp op) noexcept : op(op) {}
};
#define decl_binary_op_node(name) \
struct Mir##name##Op : MirOperate {\
    std::shared_ptr<MirExpr> lhs,rhs;\
    explicit  Mir##name##Op() noexcept : MirOperate(MirOp::name) {}\
};

decl_binary_op_node(IAdd)
decl_binary_op_node(ISub)
decl_binary_op_node(IMul)
decl_binary_op_node(IDiv)
decl_binary_op_node(IMod)
decl_binary_op_node(IPow)

decl_binary_op_node(Le)
decl_binary_op_node(Lt)
decl_binary_op_node(Ge)
decl_binary_op_node(Gt)
decl_binary_op_node(Eq)
decl_binary_op_node(Ne)
decl_binary_op_node(And)
decl_binary_op_node(Or)

struct MirStmt : MirNode {
    enum class Kind { Expr, TmpAssign, Assign };
    Kind kind;
};

struct MirExprStmt : MirStmt {
    std::shared_ptr<MirExpr> expr;
};

struct MirTmpAssignStmt : MirStmt {
    std::string name;
    std::shared_ptr<MirExpr> expr;
};

struct MirAssignStmt : MirStmt {
    std::string name;
    std::shared_ptr<MirExpr> expr;
};
struct MirFunctionStmt : MirStmt {
    std::string name;
    std::vector<std::shared_ptr<MirStmt>> body;
    std::vector<std::pair<std::string, std::shared_ptr<MirType>>> args;
};

using MirProgram = std::vector<std::shared_ptr<MirStmt>>;

std::string to_string(runtime::ValueKind type);
std::string to_string(MirOp op);
std::string to_string(const MirExpr &expr);
std::string to_string(const MirValue &lit);
std::string to_string(const MirOperate &op);
std::string to_string(const MirStmt &stmt);
std::string to_string(const MirProgram &prog);

}
#undef decl_binary_op_node