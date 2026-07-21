//
// Created by meian on 2026/7/20.
//

#pragma once
#include <vector>

#include "../runtime/opcode.hpp"

namespace lmx {

class InstEmitter {
public:
    using InstSeq = std::vector<uint8_t[4]>;
    static void emit(std::vector<uint8_t[4]>& s, runtime::Opcode::Opcode op, uint8_t  a, uint8_t  b, uint8_t c) noexcept;
    static void emit(std::vector<uint8_t[4]>& s, runtime::Opcode::Opcode op, uint16_t a, uint8_t  b) noexcept;
    static void emit(std::vector<uint8_t[4]>& s, runtime::Opcode::Opcode op, uint8_t  a, uint16_t b) noexcept;
    static void emit(std::vector<uint8_t[4]>& s, runtime::Opcode::Opcode op, uint8_t  a) noexcept;
};

}
