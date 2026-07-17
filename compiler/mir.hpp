//
// Created by meian on 2026/7/16.
//

#pragma once
#include <memory>
#include <string>
#include <vector>

namespace lmx::mir {

enum class MirType { Int, Frac, Bool, String };

struct MirRef {
    MirType type;
    bool is_temp;
    std::string name;
};

enum class MirOp {
    Add, Sub, Mul, Div,
    Le, Lt, Ge, Gt, Eq, Ne,
    And, Or
};

struct MirNode {
    virtual ~MirNode() = default;
};

struct MirExpr : MirNode {
    enum class Kind { Literal, Operate, Ref };
    Kind kind;
    MirType type;
};

struct MirLiteral : MirExpr {
    std::string data;
};

struct MirOperate : MirExpr {
    MirOp op;
    std::vector<MirRef> args;
};

struct MirRefExpr : MirExpr {
    MirRef ref;
};

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

using MirProgram = std::vector<std::shared_ptr<MirStmt>>;

std::string to_string(MirType type);
std::string to_string(MirOp op);
std::string to_string(const MirRef &ref);
std::string to_string(const MirExpr &expr);
std::string to_string(const MirLiteral &lit);
std::string to_string(const MirOperate &op);
std::string to_string(const MirRefExpr &ref);
std::string to_string(const MirStmt &stmt);
std::string to_string(const MirProgram &prog);

}
