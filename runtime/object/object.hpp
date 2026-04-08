#pragma once
#include <string>

namespace lmx::runtime {

enum class ObjectKind {
    Object,
    Code,
    String,
    Table,
    Fraction,
    Vector,
    Matrix,
};
class Object {
    ObjectKind kind { ObjectKind::Object };

public:
    explicit Object(ObjectKind kind) noexcept;
    virtual ~Object() noexcept;

    [[nodiscard]] virtual Object*       clone       () const noexcept = 0;
    [[nodiscard]] virtual std::string   to_string   () const noexcept = 0;
    [[nodiscard]] virtual std::string   type_info   () const noexcept = 0;
    [[nodiscard]] ObjectKind            get_kind    () const noexcept;
    [[nodiscard]] virtual bool          equals(const Object* other) const noexcept = 0;

    [[nodiscard]] virtual bool operator==(const Object& other) const noexcept = 0;
    [[nodiscard]] virtual bool operator!=(const Object& other) const noexcept = 0;
};
}