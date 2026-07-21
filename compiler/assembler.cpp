//
// Created by meian on 2026/7/20.
//

#include "assembler.hpp"

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

}
