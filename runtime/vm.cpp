//
// Created by meian on 2026/4/6.
//

#include "vm.hpp"

#include "opcode.hpp"

using namespace lmx::runtime;

LaminaVM::LaminaVM() noexcept = default;

LaminaVM::LaminaVM(ConstantPoolInfo *cp, Value *stack) noexcept : cp(cp), stack(stack) {init();}

LaminaVM::LaminaVM(int argc, char **argv) noexcept : args(argv, argc) {init();}

Frame::Frame(Frame *last, uint8_t *ret_addr, const size_t var_count) noexcept
    : last(last), ret_addr(ret_addr), local_vars(new Value[var_count]) {}

void LaminaVM::init() noexcept {
    this->frame = std::make_unique<Frame>(nullptr, nullptr, 128);
}

int LaminaVM::run(Code *new_prog) noexcept {
    this->prog = new_prog;
    this->ip = prog->code;
    run_continue:
    switch (*reinterpret_cast<Opcode::Opcode *>(ip)) {
    case Opcode::Nop: {
        ip++;
        goto run_continue;
    }
    case Opcode::New: {
        auto type = cp[*(uint16_t*)(ip + 1)].type;
        // todo!
        ip += 3;
        goto run_continue;
    }
    case Opcode::GetTrue: {
        *(++stack) = Value(true);
        ip++;
        goto run_continue;
    }
    case Opcode::GetFalse: {
        *(++stack) = Value(false);
        ip++;
        goto run_continue;
    }
    case Opcode::GetNull: {
        *(++stack) = Value();
        ip++;
        goto run_continue;
    }
    case Opcode::IConst: {
        *(++stack) = Value(static_cast<int64_t>(*reinterpret_cast<int *>(ip + 1)));
        ip+=5;
        goto run_continue;
    }
    case Opcode::ICConst: {
        *(++stack) = Value(cp[*(uint16_t*)(ip + 1)].int_value);
        ip+=3;
        goto run_continue;
    }
    case Opcode::Pop: {
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::Halt: {
        return 0;
    }
    default: return -1;
    }
    return 0;
}
