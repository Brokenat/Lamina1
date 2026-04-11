//
// Created by meian on 2026/4/6.
//

#include "vm.hpp"

#include <cmath>
#include <utility>

#include "opcode.hpp"

using namespace lmx::runtime;
static Value static_global_stack[65536];
static Value static_global_vars [65536];
#define LMX_LOCAL_VAR_COUNT 256
LaminaVM::LaminaVM(ConstantPoolInfo *cp, const int argc, char **argv) noexcept :
    cp(cp),
    stack(static_global_stack),
    local_vars_bp(new Value[LMX_LOCAL_VAR_COUNT * 100]),
    local_vars_curp(local_vars_bp),
    global_vars(static_global_vars),
    cur_frame(new Frame(nullptr, nullptr, local_vars_curp)),
    args(argv, argc) {}

LaminaVM::~LaminaVM() noexcept {
    delete[] local_vars_bp;
    for (const auto frames : free_frames) delete frames;
    delete cur_frame;
}


void LaminaVM::new_frame(uint8_t *ret_addr) noexcept {
    local_vars_curp += LMX_LOCAL_VAR_COUNT;
    if (free_frames.empty()) {
        cur_frame = new Frame(cur_frame, ret_addr, local_vars_curp);
        //cur_frame = frame;
        return;
    }
    const auto frame = free_frames.back();
    free_frames.pop_back();
    frame->last = cur_frame;
    frame->local_vars = local_vars_curp;
    frame->ret_addr = ret_addr;
    cur_frame = frame;
}

uint8_t* LaminaVM::pop_frame() noexcept {
    local_vars_curp -= LMX_LOCAL_VAR_COUNT;
    free_frames.push_back(cur_frame);
    const auto ret = cur_frame->ret_addr;
    cur_frame = cur_frame->last;
    return ret;
}

Frame::Frame(Frame* last, uint8_t *ret_addr, Value* local_vars) noexcept
    : last(last), ret_addr(ret_addr), local_vars(local_vars) {}

Frame::~Frame() noexcept = default;

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
        // auto type = cp[*(uint16_t*)(ip + 1)].type;
        // todo!
        ip += 3;
        goto run_continue;
    }
    case Opcode::GetTrue: {
        *++stack = true;
        ip++;
        goto run_continue;
    }
    case Opcode::GetFalse: {
        *++stack = false;
        ip++;
        goto run_continue;
    }
    case Opcode::GetNull: {
        *++stack = Value();
        ip++;
        goto run_continue;
    }
    case Opcode::IConst: {
        *++stack = static_cast<int64_t>(*reinterpret_cast<int *>(ip + 1));
        ip+=5;
        goto run_continue;
    }
    case Opcode::ICConst: {
        *(++stack) = cp[*reinterpret_cast<uint16_t *>(ip + 1)].int_value;
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
    case Opcode::IAdd: {
        *(stack - 1) += *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::ISub: {
        *(stack - 1) -= *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::IMul: {
        *(stack - 1) *= *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::IDiv: {
        *(stack - 1) /= *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::IMod: {
        *(stack - 1) %= *stack;
    }
    case Opcode::Goto: {
        ip += *(ip + 1);
        goto run_continue;
    }
    case Opcode::LGet: {
        *++stack = cur_frame->local_vars[*(ip + 1)];
        ip+=2;
        goto run_continue;
    }
    case Opcode::LSet: {
        cur_frame->local_vars[*(ip + 1)] = *stack--;
        ip+=2;
        goto run_continue;
    }
    case Opcode::GGet: {
        *++stack = global_vars[*reinterpret_cast<uint16_t*>(ip + 1)];
        ip+=3;
        goto run_continue;
    }
    case Opcode::GSet: {
        global_vars[*reinterpret_cast<uint16_t*>(ip + 1)] = *stack--;
        ip+=3;
        goto run_continue;
    }
    case Opcode::Call: {
        new_frame(ip + 5);
        ip = prog->code + *reinterpret_cast<uint32_t*>(ip + 1);
        goto run_continue;
    }
    case Opcode::Ret: {
        ip = pop_frame();
        goto run_continue;
    }
    case Opcode::IPow: {
        (stack - 1)->int_val = std::pow((stack - 1)->int_val, stack->int_val);
        ip++;
        goto run_continue;
    }
    case Opcode::FuncCReate: {
        ip++;
        goto run_continue;
    }
    case Opcode::SubCall:
    case Opcode::CCall:
    case Opcode::CmpEq: {
        *(stack - 1) = *(stack - 1) == *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::CmpNe: {
        *(stack - 1) = *(stack - 1) != *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::CmpLt: {
        *(stack - 1) = *(stack - 1) <= *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::CmpLe: {
        *(stack - 1) = *(stack - 1) < *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::CmpGt: {
        *(stack - 1) = *(stack - 1) > *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::CmpGe: {
        *(stack - 1) = *(stack - 1) >= *stack;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::IfTrue: {
        if (*stack--) ip += *(ip + 1);
        else ip+=2;
        goto run_continue;
    }
    case Opcode::IfFalse: {
        if (!(*stack--)) ip += *(ip + 1);
        else ip+=2;
        goto run_continue;
    }
    case Opcode::INeg: {
        *stack = -*stack;
        ip++;
    }
    case Opcode::FAddi: {
        (stack - 1)->frac_val += stack->int_val;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FSUbi: {
        (stack - 1)->frac_val -= stack->int_val;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FMuli: {
        (stack - 1)->frac_val *= stack->int_val;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FDivi: {
        (stack - 1)->frac_val /= stack->int_val;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FModi: {
        *(stack - 1) = (stack - 1)->frac_val % stack->int_val;
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FPow: {
        (stack - 1)->frac_val.num = std::pow(
                (stack - 1)->frac_val.num,
                stack->int_val);
        (stack - 1)->frac_val.den = std::pow(
            (stack - 1)->frac_val.den,
            stack->int_val);
        stack--;
        ip++;
        goto run_continue;
    }
    case Opcode::FNeg: {
        stack->frac_val.num = -stack->frac_val.num;
        ip++;
        goto run_continue;
    }
        //default: return -1;
    }
    std::unreachable();
}
