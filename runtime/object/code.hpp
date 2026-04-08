//
// Created by meian on 2026/4/6.
//

#pragma once
#include <cstdint>

#include "object.hpp"

namespace lmx::runtime {

class Code : Object {
public:
    uint16_t constant_id;
    uint8_t* code;

    explicit Code(uint16_t constant_id, uint8_t* code) noexcept;

    [[nodiscard]] Object*       clone       () const noexcept override;
    [[nodiscard]] std::string   to_string   () const noexcept override;
    [[nodiscard]] std::string   type_info   () const noexcept override;
    [[nodiscard]] bool          equals(const Object* other) const noexcept override;

    [[nodiscard]] bool operator==(const Object& other) const noexcept override;
    [[nodiscard]] bool operator!=(const Object& other) const noexcept override;

};

}
