//
// Created by meian on 2026/7/20.
//

#pragma once
#include <cstdint>
#include <vector>

#include "../runtime/opcode.hpp"

namespace lmx {



class InstEmitter {
public:
    struct InstWord {
        uint8_t bytes[4];
    };
    using InstSeq = std::vector<InstWord>;
    static void emit(InstSeq& s, runtime::Opcode::Opcode op, uint8_t  a, uint8_t  b, uint8_t c) noexcept;
    static void emit(InstSeq& s, runtime::Opcode::Opcode op, uint8_t  a, uint8_t  b) noexcept;
    static void emit(InstSeq& s, runtime::Opcode::Opcode op, uint16_t a, uint8_t  b) noexcept;
    static void emit(InstSeq& s, runtime::Opcode::Opcode op, uint8_t  a, uint16_t b) noexcept;
    static void emit(InstSeq& s, runtime::Opcode::Opcode op, uint8_t  a) noexcept;
};

}
