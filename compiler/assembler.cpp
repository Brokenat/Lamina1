//
// Created by meian on 2026/7/20.
//

#include "assembler.hpp"

#include "lmx.h"

namespace lmx {

void InstEmitter::emit(InstSeq& s, const runtime::Opcode::Opcode op, const uint8_t a, const uint8_t b, const uint8_t c) noexcept {
    s.push_back({{static_cast<uint8_t>(op), a, b, c}});
}

void InstEmitter::emit(InstSeq& s, const runtime::Opcode::Opcode op, const uint8_t a, const uint8_t b) noexcept {
    s.push_back({{static_cast<uint8_t>(op), a, b, 0}});
}

void InstEmitter::emit(InstSeq& s, const runtime::Opcode::Opcode op, const uint16_t a, const uint8_t b) noexcept {
    s.push_back({{
        static_cast<uint8_t>(op),
        static_cast<uint8_t>(a & 0xFF),
        static_cast<uint8_t>((a >> 8) & 0xFF),
        b
    }});
}

void InstEmitter::emit(InstSeq& s, const runtime::Opcode::Opcode op, const uint8_t a, const uint16_t b) noexcept {
    s.push_back({{
        static_cast<uint8_t>(op),
        a,
        static_cast<uint8_t>(b & 0xFF),
        static_cast<uint8_t>((b >> 8) & 0xFF)
    }});
}

void InstEmitter::emit(InstSeq& s, const runtime::Opcode::Opcode op, const uint8_t a) noexcept {
    s.push_back({{static_cast<uint8_t>(op), a, 0, 0}});
}

void InstEmitter::emit(InstSeq &s, runtime::Opcode::Opcode op) noexcept {
    s.push_back({{static_cast<uint8_t>(op), 0, 0, 0}});
}

bool InstEmitter::inst_is_ret_reg(const runtime::Opcode::Opcode op) noexcept {
    switch (op) {
    case runtime::Opcode::Nop:
    case runtime::Opcode::Halt:
    case runtime::Opcode::FuncCreate:
    case runtime::Opcode::CallVirtual:
    case runtime::Opcode::CCall:
    case runtime::Opcode::CallFast:
    case runtime::Opcode::Ret:
    case runtime::Opcode::Goto:
    case runtime::Opcode::IfTrue:
    case runtime::Opcode::IfFalse:
    case runtime::Opcode::Call:
        return false;
    case runtime::Opcode::New:
    case runtime::Opcode::GetTrue:
    case runtime::Opcode::GetFalse:
    case runtime::Opcode::GetNull:
    case runtime::Opcode::IAdd:
    case runtime::Opcode::ISub:
    case runtime::Opcode::IMul:
    case runtime::Opcode::IDiv:
    case runtime::Opcode::IMod:
    case runtime::Opcode::IPow:
    case runtime::Opcode::INeg:
    case runtime::Opcode::IConst:
    case runtime::Opcode::CConst:
    case runtime::Opcode::ICmpEq:
    case runtime::Opcode::ICmpNe:
    case runtime::Opcode::ICmpLt:
    case runtime::Opcode::ICmpLe:
    case runtime::Opcode::ICmpGt:
    case runtime::Opcode::ICmpGe:
    case runtime::Opcode::LGet:
    case runtime::Opcode::LSet:
    case runtime::Opcode::GGet:
    case runtime::Opcode::GSet:
    case runtime::Opcode::FAdd:
    case runtime::Opcode::FSub:
    case runtime::Opcode::FMul:
    case runtime::Opcode::FDiv:
    case runtime::Opcode::FMod:
    case runtime::Opcode::FNeg:
    case runtime::Opcode::MovRR:
    case runtime::Opcode::Pop:
        return true;
    }
    return false;
}

bool InstEmitter::inst_is_call(const runtime::Opcode::Opcode op) noexcept {
    switch (op) {
    case runtime::Opcode::Nop:
    case runtime::Opcode::Halt:
    case runtime::Opcode::FuncCreate:
    case runtime::Opcode::Ret:
    case runtime::Opcode::Goto:
    case runtime::Opcode::IfTrue:
    case runtime::Opcode::IfFalse:
    case runtime::Opcode::New:
    case runtime::Opcode::GetTrue:
    case runtime::Opcode::GetFalse:
    case runtime::Opcode::GetNull:
    case runtime::Opcode::IAdd:
    case runtime::Opcode::ISub:
    case runtime::Opcode::IMul:
    case runtime::Opcode::IDiv:
    case runtime::Opcode::IMod:
    case runtime::Opcode::IPow:
    case runtime::Opcode::INeg:
    case runtime::Opcode::IConst:
    case runtime::Opcode::CConst:
    case runtime::Opcode::ICmpEq:
    case runtime::Opcode::ICmpNe:
    case runtime::Opcode::ICmpLt:
    case runtime::Opcode::ICmpLe:
    case runtime::Opcode::ICmpGt:
    case runtime::Opcode::ICmpGe:
    case runtime::Opcode::LGet:
    case runtime::Opcode::LSet:
    case runtime::Opcode::GGet:
    case runtime::Opcode::GSet:
    case runtime::Opcode::FAdd:
    case runtime::Opcode::FSub:
    case runtime::Opcode::FMul:
    case runtime::Opcode::FDiv:
    case runtime::Opcode::FMod:
    case runtime::Opcode::FNeg:
    case runtime::Opcode::MovRR:
    case runtime::Opcode::Pop:
        return false;
    case runtime::Opcode::CCall:
    case runtime::Opcode::CallFast:
    case runtime::Opcode::Call:
    case runtime::Opcode::CallVirtual:
        return true;

    }
    return false;
}

std::optional<uint8_t> RegAllocator::alloc() noexcept {
    for (size_t i = 0; i < COMMON_REG_COUNT; i++) {
        if (!regs[i]) {
            regs.set(i);
            return static_cast<uint8_t>(i + 1);
        }
    }
    return std::nullopt;
}

void RegAllocator::free(const uint8_t reg) noexcept {
    regs.reset(reg - 1);
}

void Assembler::write_u32(std::vector<uint8_t>& buf, const uint32_t value) noexcept {
    buf.push_back(static_cast<uint8_t>(value & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
}

void Assembler::write_n(std::vector<uint8_t>& buf, const uint8_t* src, const size_t n) noexcept {
    buf.insert(buf.end(), src, src + n);
}

void Assembler::write_u64(std::vector<uint8_t>& buf, const uint64_t value) noexcept {
    buf.push_back(static_cast<uint8_t>(value & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 32) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 40) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 48) & 0xFF));
    buf.push_back(static_cast<uint8_t>((value >> 56) & 0xFF));
}

AssemblerContext::AssemblerContext() noexcept = default;

AssemblerContext::Val::Val(std::string name, const uint8_t reg, const bool is_tmp) noexcept
    : kind(Kind::Reg), is_tmp(is_tmp), name(std::move(name)), reg(reg) {}

AssemblerContext::Val::Val(std::string name, const uint8_t var) noexcept
    : kind(Kind::Var), is_tmp(false), name(std::move(name)), var(var) {}

std::vector<uint8_t> Assembler::asm_module(mir::MirModule *mod) noexcept {
    AssemblerContext c;
    std::vector<uint8_t> result;
    result.reserve(1024);

    /* write magic num */
    write_u32(result, LMX_MAGIC_NUM);
    /* write version */
    write_u32(result, LMX_VERSION);

    std::vector<uint8_t> cp;
    cp.resize(8);

    std::vector<uint8_t> funcs;
    funcs.reserve(1024);


    return result;
}

std::vector<uint8_t> Assembler::asm_func(AssemblerContext& c, mir::MirFuncDefine *def) noexcept {
    c.vals.clear();
    std::vector<uint8_t> result;
    result.resize(4);

    return result;
}

std::optional<AssemblerContext::GlobalVar*> AssemblerContext::find_global(const std::string& name) noexcept {
    for (auto& g : globals) {
        if (g.name == name) {
            return &g;
        }
    }
    return std::nullopt;
}

std::optional<AssemblerContext::Val *> AssemblerContext::find_var(const std::string& name) noexcept {
    for (auto& v : vals) {
        if (v.name == name) {
            return &v;
        }
    }
    return std::nullopt;
}

uint8_t Assembler::asm_mir_expr(AssemblerContext& c, InstEmitter::InstSeq& insts, mir::MirExpr *node) noexcept {
    switch (node->kind) {
    case mir::MirExprKind::Ref: {
        const auto e = reinterpret_cast<mir::MirRefExpr*>(node);
        const auto v = *c.find_var(e->name);
        return v->reg;
        break;
    }
    case mir::MirExprKind::Literal: {
        const auto e = reinterpret_cast<mir::MirLiteralExpr*>(node);
        const auto r = *c.reg.alloc();
        switch (e->literal_kind) {
        case mir::MirLiteralKind::Integer: {
            const auto r1 = *c.reg.alloc();
            InstEmitter::emit(insts, runtime::Opcode::IConst, r1, static_cast<uint16_t>(std::stoi(e->data)));
            return r1;
            break;
        }
        case mir::MirLiteralKind::Float: {
            const runtime::Fraction f(e->data);
            const auto r1 = *c.reg.alloc();
            const auto r2 = *c.reg.alloc();
            const auto r3 = *c.reg.alloc();
            InstEmitter::emit(insts, runtime::Opcode::IConst, r1, static_cast<uint16_t>(f.den));
            c.reg.free(r1);
            InstEmitter::emit(insts, runtime::Opcode::IConst, r2, static_cast<uint16_t>(f.num));
            c.reg.free(r2);
            InstEmitter::emit(insts, runtime::Opcode::IDiv, r3, r2, r1);
            return r3;
            break;
        }
        case mir::MirLiteralKind::String:
            break;
        case mir::MirLiteralKind::Boolean: {
            const auto r1 = *c.reg.alloc();
            if (e->data == "true") {
                InstEmitter::emit(insts, runtime::Opcode::GetTrue, r1);
            } else {
                InstEmitter::emit(insts, runtime::Opcode::GetFalse, r1);
            }
            return r1;
            break;
        }
        }
        break;
    }
    case mir::MirExprKind::Operate: {
        const auto e = reinterpret_cast<mir::MirOperateExpr*>(node);
        switch (e->operate_kind) {
        case mir::MirOperateKind::Normal: {
            if (InstEmitter::inst_is_call(e->opcode)) {
                return 0;
            } else {
                switch (e->opcode) {
                case runtime::Opcode::New: {

                }
                case runtime::Opcode::GetTrue:
                case runtime::Opcode::GetFalse:
                case runtime::Opcode::GetNull:
                case runtime::Opcode::IAdd:
                case runtime::Opcode::ISub:
                case runtime::Opcode::IMul:
                case runtime::Opcode::IDiv:
                case runtime::Opcode::IMod:
                case runtime::Opcode::IPow:
                case runtime::Opcode::INeg:
                case runtime::Opcode::IConst:
                case runtime::Opcode::CConst:
                case runtime::Opcode::ICmpEq:
                case runtime::Opcode::ICmpNe:
                case runtime::Opcode::ICmpLt:
                case runtime::Opcode::ICmpLe:
                case runtime::Opcode::ICmpGt:
                case runtime::Opcode::ICmpGe:
                case runtime::Opcode::LGet:
                case runtime::Opcode::LSet:
                case runtime::Opcode::GGet:
                case runtime::Opcode::GSet:
                case runtime::Opcode::FAdd:
                case runtime::Opcode::FSub:
                case runtime::Opcode::FMul:
                case runtime::Opcode::FDiv:
                case runtime::Opcode::FMod:
                case runtime::Opcode::FNeg:
                case runtime::Opcode::MovRR:
                case runtime::Opcode::Pop:

                default: ;
                }
            }
            break;
        }
        case mir::MirOperateKind::RetVoid: {
            InstEmitter::emit(insts, runtime::Opcode::Ret);
            return 0;
            break;
        }
        }
        break;
    }
    }
}

InstEmitter::InstSeq Assembler::asm_mir_node(AssemblerContext& c, mir::MirNode *node) noexcept {
    InstEmitter::InstSeq result;

    switch (node->kind) {
    case mir::MirNodeKind::TempAssign: {
        const auto n = reinterpret_cast<mir::MirTempAssign*>(node);

        break;
    }
    case mir::MirNodeKind::Assign:
        break;
    case mir::MirNodeKind::Label:
        break;
    case mir::MirNodeKind::Expr:
        break;
    case mir::MirNodeKind::Func:
        break;
    }
    return result;
}
}
