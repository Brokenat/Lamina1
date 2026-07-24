//
// Created by meian on 2026/4/6.
//

#include "code_module.hpp"
#include "lmx.h"

using namespace lmx::runtime;

FuncObj::FuncObj(CodeModule *mod, const uint8_t *addr) noexcept
   : mod(mod), addr(addr) {}

namespace {
class ModuleLoader {
public:
    static bool check_magic(const uint8_t*& p) noexcept {
        using Magic = decltype(LMX_MAGIC_NUM);
        // if (p == nullptr) return false;
        if (*reinterpret_cast<const Magic*>(p) != LMX_MAGIC_NUM) return false;
        p += sizeof(Magic);
        return true;
    }
    static bool check_version(const uint8_t*& p) noexcept {
        using Version = decltype(LMX_VERSION);
        // if (p == nullptr) return false;
        if (*reinterpret_cast<const Version*>(p) != LMX_VERSION) return false;
        p += sizeof(Version);
        return true;
    }
    static bool load_cp(std::vector<ConstantPoolInfo>& result, const uint8_t*& p) noexcept {
        const auto size = *reinterpret_cast<const uint64_t*>(p);
        p += sizeof(uint64_t);
        const auto over = p + size;
        while (p != over) {
            switch (static_cast<ConstantId>(*p++)) {
            case ConstantId::Int: {
                using IdType = int64_t;
                result.emplace_back(*reinterpret_cast<const IdType*>(p));
                p += sizeof(IdType);
                break;
            }
            case ConstantId::Frac: {
                using IdType = FracInfo;
                result.emplace_back(reinterpret_cast<const IdType*>(p));
                p += sizeof(IdType);
                break;
            }
            case ConstantId::Str: {
                using IdType = StringInfo;
                result.emplace_back(reinterpret_cast<const IdType*>(p));
                p += sizeof(IdType);
                p += result.back().str->length;
                break;
            }
            }
        }
        return true;
    }
    static bool load_funcs(CodeModule* mod, std::vector<FuncObj>& result, const uint8_t*& p) noexcept {
        const auto over = p + *reinterpret_cast<const uint64_t*>(p) + sizeof(uint64_t);
        p += sizeof(uint64_t);
        while (p != over) {
            const auto len = *reinterpret_cast<const uint32_t*>(p);
            p += sizeof(uint32_t);
            result.emplace_back(mod, p);
            p += len;
        }
        return true;
    }

};
}



Object *CodeModule::clone() const noexcept {
    return nullptr;
}

CodeModule::CodeModule(std::vector<ConstantPoolInfo> cp, std::vector<TypeInfo> types, std::vector<FuncObj> funcs, const uint8_t *code) noexcept
    : Object(ObjectKind::Code), cp(std::move(cp)), funcs(std::move(funcs)), types(std::move(types)), code(code) {}

CodeModule::CodeModule(std::vector<ConstantPoolInfo>&& cp, std::vector<TypeInfo>&& types, std::vector<FuncObj>&& funcs, const uint8_t *code) noexcept
    : Object(ObjectKind::Code), cp(std::move(cp)), funcs(std::move(funcs)), types(std::move(types)), code(code) {}

CodeModule::CodeModule(const uint8_t *binary) noexcept : Object(ObjectKind::Code) {
    ModuleLoader::check_magic(binary);
    ModuleLoader::check_version(binary);
    ModuleLoader::load_funcs(this, funcs, binary);
    ModuleLoader::load_cp(cp, binary);
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

