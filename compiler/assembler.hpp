//
// Created by meian on 2026/7/20.
//

#pragma once
#include <bitset>
#include <cstdint>
#include <optional>
#include <vector>

#include "mir.hpp"
#include "../runtime/opcode.hpp"
#include "../runtime/vm.hpp"

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

    static bool inst_is_ret_reg(runtime::Opcode::Opcode op) noexcept;
};
class RegAllocator {

    // r0 cannot be used
    static constexpr size_t COMMON_REG_COUNT = static_cast<size_t>(LMX_VM_REG_COUNT) - 1;
    std::bitset<COMMON_REG_COUNT> regs;

public:
    std::optional<uint8_t> alloc() noexcept;
    void free(uint8_t reg) noexcept;
};
class Assembler {
public:
    static void write_u32(std::vector<uint8_t>& buf, uint32_t value) noexcept;
    static void write_u64(std::vector<uint8_t>& buf, uint64_t value) noexcept;
    static void write_n(std::vector<uint8_t>& buf, const uint8_t* src, size_t n) noexcept;
    static std::vector<uint8_t> asm_module(mir::MirModule* mod) noexcept;

    static std::vector<uint8_t> asm_func(mir::MirFuncDefine *def) noexcept;
};

}
