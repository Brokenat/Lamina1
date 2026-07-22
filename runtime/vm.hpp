//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>
#include <vector>
#include <span>

#include "binary.hpp"
#include "gc.hpp"
#include "lmx.h"
#include "object/code_module.hpp"
#include "object/value.hpp"

namespace lmx::runtime {

#define LMX_LOCAL_VAR_COUNT 256
#define LMX_CALLSTACK_MAX_COUNT 100
#define LMX_VM_REG_COUNT 256

struct Frame {
    Frame* last;
    const uint8_t* ret_addr;
    Value* local_vars;
    explicit Frame(Frame* last, const uint8_t* ret_addr, Value* local_vars) noexcept;
    ~Frame() noexcept;
};
class LaminaVM {
    Value regs[LMX_VM_REG_COUNT];
    ConstantPoolInfo* cp;
    Value* stack;
    CodeModule* prog          {nullptr};
    Value* local_vars_bp;
    Value* local_vars_curp;
    Value* global_vars;
    std::vector<Frame*> free_frames;
    Frame* cur_frame;
    LmGCAllocator allocator{};

    std::span<char*> args;


public:
    explicit LaminaVM() noexcept = delete;
    explicit LaminaVM(ConstantPoolInfo* cp, int argc, char** argv) noexcept;
    ~LaminaVM() noexcept;
    int run(CodeModule* prog) noexcept;

    friend LMX_INLINE void new_frame(LaminaVM* vm, const uint8_t *ret_addr) noexcept {
        if (vm->free_frames.empty()) {
            vm->cur_frame = new Frame(vm->cur_frame, ret_addr, vm->local_vars_curp);
            //cur_frame = frame;
            //return;
        } else {
            const auto frame = vm->free_frames.back();
            vm->free_frames.pop_back();
            new (frame) Frame(vm->cur_frame, ret_addr, vm->local_vars_curp);
            vm->cur_frame = frame;
        }
        vm->local_vars_curp += LMX_LOCAL_VAR_COUNT;
    }
    friend LMX_INLINE const uint8_t *pop_frame(LaminaVM* vm) noexcept {
        auto* cur_frame = vm->cur_frame;
        vm->local_vars_curp -= LMX_LOCAL_VAR_COUNT;
        auto i = 0;
        while (i <= LMX_LOCAL_VAR_COUNT) {
            (vm->local_vars_curp + i)->~Value();
            i++;
        }
        vm->free_frames.push_back(cur_frame);
        vm->cur_frame = cur_frame->last;
        return cur_frame->ret_addr;
    }
};


}
