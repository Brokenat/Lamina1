#pragma once
#include "object.hpp"

namespace lmx::runtime {

class Fraction : public Object {
    mutable int64_t num {0};
    mutable int64_t den {1};
public:
    explicit Fraction() noexcept;

    void simplify() const noexcept;
    explicit Fraction(int64_t num, int64_t den) noexcept;
    explicit Fraction(const std::string& num_str) noexcept;

    ~Fraction() noexcept override = default;

    [[nodiscard]] Object* clone() const noexcept override;
    [[nodiscard]] std::string to_string() const noexcept override;
    [[nodiscard]] bool equals(const Object* other) const noexcept override;
    [[nodiscard]] std::string type_info() const noexcept override;

    [[nodiscard]] Fraction operator+(const Fraction& other) const noexcept;
    [[nodiscard]] Fraction operator-(const Fraction& other) const noexcept;
    [[nodiscard]] Fraction operator*(const Fraction& other) const noexcept;
    [[nodiscard]] Fraction operator/(const Fraction& other) const noexcept;

    [[nodiscard]] bool operator==(const Object& other) const noexcept override;
    [[nodiscard]] bool operator!=(const Object& other) const noexcept override;
};
}