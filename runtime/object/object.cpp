//
// Created by meian on 2026/3/28.
//
#include "object.hpp"

using namespace lmx::runtime;

Object::Object(const ObjectKind kind) noexcept: kind(kind) {}


Object::~Object() noexcept = default;

ObjectKind Object::get_kind() const noexcept {
    return this->kind;
}
