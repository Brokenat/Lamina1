//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <span>

#include "binary.hpp"
#include "object/code.hpp"
#include "object/value.hpp"



namespace lmx::runtime {

struct Frame {
    Frame* last;
    uint8_t* ret_addr;
    Value* local_vars;
    explicit Frame(Frame* last, uint8_t* ret_addr, Value* local_vars) noexcept;
    ~Frame() noexcept;
};

class LaminaVM {
    ConstantPoolInfo* cp;
    Value* stack;
    Code* prog          {nullptr};
    uint8_t* ip         {nullptr};
    Value* local_vars_bp;
    Value* local_vars_curp;
    Value* global_vars;
    std::vector<Frame*> free_frames;
    Frame* cur_frame;

    std::span<char*> args;

    void new_frame(uint8_t* ret_addr) noexcept;
    uint8_t* pop_frame() noexcept;
public:
    explicit LaminaVM() noexcept = delete;
    explicit LaminaVM(ConstantPoolInfo* cp, int argc, char** argv) noexcept;
    ~LaminaVM() noexcept;
    int run(Code* prog) noexcept;
};

}
