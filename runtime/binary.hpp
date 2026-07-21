//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>
#include <vector>
namespace lmx::runtime {

constexpr uint32_t MAGIC_NUM = 0x434D4C00;
enum class ConstantId : uint8_t {
    Int, Frac, Str
};
struct FracInfo {
    int32_t num;
    int32_t den;
};
struct StringInfo {
    uint32_t length;
#if defined(_MSC_VER)
    char* str;
    explicit StringInfo(uint32_t len, decltype(str) str) noexcept;
#else
    char str[];
#endif

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
        StringInfo* str;
    };

    explicit ConstantPoolInfo(decltype(int_value) int_value) noexcept;
    explicit ConstantPoolInfo(decltype(frac_info) frac_info) noexcept;
    explicit ConstantPoolInfo(decltype(str) str) noexcept;

#if defined(_MSC_VER)
    ~ConstantPoolInfo() noexcept;
#endif
};


struct BinaryModule {
    std::vector<ConstantPoolInfo> constant_pool;
};
}