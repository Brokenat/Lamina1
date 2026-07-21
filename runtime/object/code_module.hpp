//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>

#include "object.hpp"
#include "../binary.hpp"

namespace lmx::runtime {

class CodeModule : public Object {

public:
    std::vector<ConstantPoolInfo> cp;
    std::vector<TypeInfo> types;
    std::vector<uint8_t*> funcs;
    uint8_t* code;
    explicit CodeModule(
        std::vector<ConstantPoolInfo> cp,
        std::vector<TypeInfo> types,
        std::vector<uint8_t*> funcs,
        uint8_t* code
        ) noexcept;
    explicit CodeModule(
        std::vector<ConstantPoolInfo>&& cp,
        std::vector<TypeInfo>&& types,
        std::vector<uint8_t*>&& funcs,
        uint8_t* code
        ) noexcept;
    explicit CodeModule(uint8_t* binary) noexcept;
    [[nodiscard]] Object*       clone       () const noexcept override;
    [[nodiscard]] std::string   to_string   () const noexcept override;
    [[nodiscard]] std::string   type_info   () const noexcept override;
    [[nodiscard]] bool          equals(const Object* other) const noexcept override;

    [[nodiscard]] bool operator==(const Object& other) const noexcept override;
    [[nodiscard]] bool operator!=(const Object& other) const noexcept override;
};

}
