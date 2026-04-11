//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>

namespace lmx::runtime::Opcode {

enum Opcode : uint8_t {
    Nop,    // 0
    New,    // constant_tag_idx(2)

    GetTrue,    // 0
    GetFalse,   // 0
    GetNull,    // 0

    IConst,     // imm(4)
    ICConst,    // constant_tag_idx(2)
    Pop,        // 0
    Halt,
    IAdd, ISub, IMul, IDiv, IMod, IPow, INeg,    // 0

    FuncCReate,     // constant_tag_idx(2)

    SubCall, //
    CCall,
    Call,
    Ret,    //
    Goto,   // ip+(1)
    CmpEq, CmpNe, CmpLt, CmpLe, CmpGt, CmpGe,  // 0
    IfTrue, IfFalse, // then ip+(1)  else ip+(1)

    LGet, LSet, // idx(1)
    GGet, GSet, // idx(2)
    FAddi, FSUbi, FMuli, FDivi, FModi, FPow, FNeg,
};

}
