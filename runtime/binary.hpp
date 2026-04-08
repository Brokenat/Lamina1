//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>
namespace lmx::runtime {

constexpr uint32_t MAGIC_NUM = 0x434D4C00;
enum class ConstantId : uint8_t {
    Int, Frac, Str, Type,
};
struct FracInfo {
    int64_t num;
    int64_t den;
};
enum class TypeTag : uint16_t {
    Func,
};
struct TypeInfo {

};
struct ConstantPoolInfo {
    ConstantId id;
    union {
        int64_t int_value;
        FracInfo* frac_info;
        TypeInfo* type;
        char* str;
    };
};

}