//
// Created by meian on 2026/4/10.
//

#pragma once
#include <list>
#include <cstdint>
#include <memory>

#include "object/object.hpp"

namespace lmx::runtime {
class GC;

using GCObject = std::shared_ptr<Object>;
class LmGCAllocator {
    std::list<Object*> objects;
public:
    Object* alloc_string(const char* str) noexcept;

    ~LmGCAllocator() noexcept;
};
}