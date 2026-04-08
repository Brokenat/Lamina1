//
// Created by meian on 2026/3/29.
//

#include "value.hpp"
#include <cassert>

#include <gtest/gtest-param-test.h>

using namespace lmx::runtime;

Value::Value() noexcept {}

Value::Value(const bool bool_val) noexcept : kind(ValueKind::Bool), bool_val(bool_val) {}

Value::Value(void *c_ptr) noexcept : kind(ValueKind::C_Ptr), c_ptr(c_ptr) {}

Value::Value(const int64_t int_val) noexcept : kind(ValueKind::Int), int_val(int_val) {}

Value::Value(Object *obj) noexcept : kind(ValueKind::Obj), obj(obj) {}

Object *Value::operator->() const noexcept {
    assert(this->kind == ValueKind::Obj);
    return obj;
}

Value &Value::operator=(void *ptr) noexcept {
    assert(this->kind == ValueKind::C_Ptr);
    this->kind = ValueKind::C_Ptr;
    this->c_ptr = ptr;
    return *this;
}

Value &Value::operator=(const bool val) noexcept {
    assert(this->kind == ValueKind::Bool);
    this->kind = ValueKind::Bool;
    this->bool_val = val;
    return *this;
}

Value &Value::operator=(const int64_t val) noexcept {
    assert(this->kind == ValueKind::Int);
    this->kind = ValueKind::Int;
    this->int_val = val;
    return *this;
}

Value &Value::operator=(Object *obj) noexcept {
    assert(this->kind == ValueKind::Obj);
    this->kind = ValueKind::Obj;
    this->obj = obj;
    return *this;
}

std::string Value::to_string() const noexcept {
    switch (kind) {
    case ValueKind::Bool: return bool_val ? "true" : "false";
    case ValueKind::Int: return std::to_string(int_val);
    case ValueKind::Obj: return obj->to_string();
    case ValueKind::C_Ptr: return "<>";
    case ValueKind::Null: return "null";
    }

    // 不可能到达这里
    return {};
}

Value Value::operator%(const Value &other) const noexcept {
    return Value(this->int_val % other.int_val);
}

Value &Value::operator%=(const Value &other) noexcept {
    this->int_val %= other.int_val;
    return *this;
}

Value Value::operator*(const Value &other) const noexcept {
    return Value(this->int_val * other.int_val);
}

Value &Value::operator*=(const Value &other) noexcept {
    this->int_val *= other.int_val;
    return *this;
}

Value Value::operator/(const Value &other) const noexcept {
    return Value(this->int_val / other.int_val);
}

Value &Value::operator/=(const Value &other) noexcept {
    this->int_val /= other.int_val;
    return *this;
}

Value Value::operator+(const Value &other) const noexcept {
    return Value(int_val + other.int_val);
}

Value &Value::operator+=(const Value &other) noexcept {
    this->int_val += other.int_val;
    return *this;
}

Value Value::operator-(const Value &other) const noexcept {
    return Value(int_val - other.int_val);
}

Value &Value::operator-=(const Value &other) noexcept {
    this->int_val -= other.int_val;
    return *this;
}

bool Value::operator<(const Value &other) const noexcept {
    return int_val < other.int_val;
}

bool Value::operator<=(const Value &other) const noexcept {
    return int_val <= other.int_val;
}

bool Value::operator>(const Value &other) const noexcept {
    return int_val > other.int_val;
}

bool Value::operator>=(const Value &other) const noexcept {
    return int_val >= other.int_val;
}

bool Value::operator==(const Value &other) const noexcept {
    return int_val == other.int_val;
}

bool Value::operator!=(const Value &other) const noexcept {
    return int_val != other.int_val;
}

bool Value::operator!() const noexcept {
    return !bool_val;
}

Value::operator bool() const noexcept {
    return bool_val;
}
