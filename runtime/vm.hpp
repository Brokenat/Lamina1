//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>
#include <memory>
#include <span>

#include "binary.hpp"
#include "object/code.hpp"
#include "object/value.hpp"

namespace lmx::runtime {

struct Frame {
    Frame* last;
    uint8_t* ret_addr;
    std::unique_ptr<Value[]> local_vars;
    explicit Frame(Frame* last, uint8_t* ret_addr, size_t var_count) noexcept;
};

class LaminaVM {
    ConstantPoolInfo* cp{nullptr};
    Value* stack        {nullptr};
    Code* prog          {nullptr};
    uint8_t* ip         {nullptr};
    std::unique_ptr<Frame> frame{nullptr};
    std::span<char*> args;
public:
    explicit LaminaVM() noexcept;
    explicit LaminaVM(ConstantPoolInfo* cp, Value* stack) noexcept;
    explicit LaminaVM(int argc, char** argv) noexcept;
    void init() noexcept;
    int run(Code* prog) noexcept;
};

}
