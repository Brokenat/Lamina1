#include "fraction.hpp"
#include <cmath>
#include <cstdint>

using namespace lmx::runtime;
Fraction::Fraction() noexcept
: Object(ObjectKind::Fraction) {}

Fraction::Fraction(const int64_t num, const int64_t den) noexcept
: Object(ObjectKind::Fraction), num(num), den(den) {simplify();}

Fraction::Fraction(const std::string& num_str) noexcept
: Object(ObjectKind::Fraction) {
    std::string num_s;
    std::string den_s;
    std::string* target = &num_s;
    bool has_dot = false;
    for (const char c : num_str) {
        if (isdigit(c)) *target += c;
        else if (c == '.') {
            if (has_dot) break;
            has_dot = true;
            target = &den_s;
        }
        else if (isspace(c)) continue;
        else break;
    }

    this->den = std::pow(10, den_s.size());

    this->num = std::stoll(num_s + den_s);
    simplify();
}
static int64_t private_gcd(int64_t a, int64_t b) noexcept {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;

    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
static int64_t private_lcm(int64_t a, int64_t b) noexcept {
    if (a == 0 || b == 0) return 0;
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;

    int64_t lcm = a;
    int64_t step = a;

    while (lcm % b != 0) {
        if (lcm > INT64_MAX - step) return 0;
        lcm += step;
    }
    return lcm;
}

void Fraction::simplify() const noexcept {
    if (den == 0) return;
    if (num == 0) {
        den = 1;
        return;
    }
    const auto g = private_gcd(num, den);
    if (g == 1) return;
    num /= g;
    den /= g;
}
std::string Fraction::to_string() const noexcept {
    simplify();

    auto result = std::to_string(num);
    if (den != 1) result += "/" + std::to_string(den);

    return result;
}

Fraction Fraction::operator*(const Fraction& other) const noexcept {
    return Fraction(num * other.num, den * other.den);
}
Fraction Fraction::operator/(const Fraction& other) const noexcept {
    return Fraction(num * other.den, den * other.num);
}

#define ReDuce const auto lcm = private_lcm(den, other.den); \
const auto new_num1 = num * (lcm / den);\
const auto new_num2 = other.num * (lcm / other.den);

Fraction Fraction::operator+(const Fraction& other) const noexcept {
    ReDuce
    return Fraction(new_num1 + new_num2, lcm);
}
Fraction Fraction::operator-(const Fraction& other) const noexcept {
    ReDuce
    return Fraction(new_num1 - new_num2, lcm);
}
#undef ReDuce



Object *Fraction::clone() const noexcept {
    return new Fraction(num, den);
}

bool Fraction::equals(const Object* other) const noexcept {
    if (other->get_kind() != this->get_kind()) return false;

    const auto temp = *reinterpret_cast<const Fraction*>(other);
    temp.simplify();
    const auto backup = *this;
    backup.simplify();
    return temp.num == backup.num && temp.den == backup.den;
}

bool Fraction::operator==(const Object& other) const noexcept {
    return equals(&other);
}
bool Fraction::operator!=(const Object& other) const noexcept {
    return !equals(&other);
}

std::string Fraction::type_info() const noexcept {
    return "frac";
}
