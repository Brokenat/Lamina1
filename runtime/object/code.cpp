//
// Created by meian on 2026/4/6.
//

#include "code.hpp"

using namespace lmx::runtime;

Object *Code::clone() const noexcept {
    return nullptr;
}

Code::Code(const uint16_t constant_id, uint8_t *code) noexcept : Object(ObjectKind::Code), constant_id(constant_id), code(code) { }

std::string Code::to_string() const noexcept {
    return type_info();
}

std::string Code::type_info() const noexcept {
    return "code";
}

bool Code::equals(const Object *other) const noexcept {
    return false;
}

bool Code::operator==(const Object &other) const noexcept {
    return false;
}

bool Code::operator!=(const Object &other) const noexcept {
    return false;
}
