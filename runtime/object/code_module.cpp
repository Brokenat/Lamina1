//
// Created by meian on 2026/4/6.
//

#include "code_module.hpp"
#include "lmx.h"

using namespace lmx::runtime;

namespace {
class ModuleLoader {
public:
    static bool check_magic(uint8_t*& p) noexcept {
        using Magic = decltype(LMX_MAGIC_NUM);
        // if (p == nullptr) return false;
        if (*reinterpret_cast<Magic*>(p) != LMX_MAGIC_NUM) return false;
        p += sizeof(Magic);
        return true;
    }
    static bool check_version(uint8_t*& p) noexcept {
        using Version = decltype(LMX_VERSION);
        // if (p == nullptr) return false;
        if (*reinterpret_cast<Version*>(p) != LMX_VERSION) return false;
        p += sizeof(Version);
        return true;
    }
    static bool load_cp(std::vector<ConstantPoolInfo>& result, uint8_t*& p) noexcept {
        const auto size = *reinterpret_cast<uint64_t*>(p);
        p += sizeof(uint64_t);
        const auto over = p + size;
        while (p != over) {
            switch (static_cast<ConstantId>(*p++)) {
            case ConstantId::Int: {
                using IdType = int64_t;
                result.emplace_back(*reinterpret_cast<IdType*>(p));
                p += sizeof(IdType);
                break;
            }
            case ConstantId::Frac: {
                using IdType = FracInfo;
                result.emplace_back(reinterpret_cast<IdType*>(p));
                p += sizeof(IdType);
                break;
            }
            case ConstantId::Str: {
                using IdType = StringInfo;
#if defined(_MSC_VER)
                const auto len = *reinterpret_cast<uint32_t*>(p);
                p += sizeof(uint32_t);
                result.emplace_back(new StringInfo(len, p));
#else
                result.emplace_back(reinterpret_cast<IdType*>(p));
                p += sizeof(IdType);
                p += result.back().str->length;
#endif
                break;
            }
            }
        }
        return true;
    }
    static bool load_funcs(std::vector<uint8_t*>& result, uint8_t*& p) noexcept {
        const auto over = p + *reinterpret_cast<uint64_t*>(p) + sizeof(uint64_t);
        while (p != over) {
            const auto len = *reinterpret_cast<uint32_t*>(p);
            p += sizeof(uint32_t);
            result.push_back(p);
            p += len;
        }
        return true;
    }

};
}

Object *CodeModule::clone() const noexcept {
    return nullptr;
}

CodeModule::CodeModule(std::vector<ConstantPoolInfo> cp, std::vector<TypeInfo> types, std::vector<uint8_t *> funcs, uint8_t *code) noexcept
    : Object(ObjectKind::Code), cp(std::move(cp)), types(std::move(types)), funcs(std::move(funcs)), code(code) {}

CodeModule::CodeModule(std::vector<ConstantPoolInfo>&& cp, std::vector<TypeInfo>&& types, std::vector<uint8_t *>&& funcs, uint8_t *code) noexcept
    : Object(ObjectKind::Code), cp(std::move(cp)), types(std::move(types)), funcs(std::move(funcs)), code(code) {}

CodeModule::CodeModule(uint8_t *binary) noexcept : Object(ObjectKind::Code) {
    ModuleLoader::check_magic(binary);
    ModuleLoader::check_version(binary);
    ModuleLoader::load_cp(cp, binary);
    ModuleLoader::load_funcs(funcs, binary);
    code = binary;
}

std::string CodeModule::to_string() const noexcept {
    return type_info();
}

std::string CodeModule::type_info() const noexcept {
    return "code";
}

bool CodeModule::equals(const Object *other) const noexcept {
    return false;
}

bool CodeModule::operator==(const Object &other) const noexcept {
    return false;
}

bool CodeModule::operator!=(const Object &other) const noexcept {
    return false;
}

