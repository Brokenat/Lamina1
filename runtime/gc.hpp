//
// Created by meian on 2026/4/10.
//

#pragma once
#include <vector>
#include <cstdint>

#include "object/object.hpp"

namespace lmx::runtime {
class GC;

class GCObject {
public:
    uint32_t rc{0};
    bool is_marked{false};
    Object* obj;
    GCObject() = delete;
    explicit GCObject(Object* obj) noexcept;
    GCObject* get() noexcept;
    void release() noexcept;

    void mark() noexcept;
    ~GCObject() noexcept;
};

class GC {
    std::vector<GCObject> objs;
    std::vector<Object*> free_list;
public:
    GC() noexcept;
    ~GC() noexcept;

    GCObject* new_obj(Object* obj) noexcept;

    // void recycle_obj(GCObject* obj) noexcept;
};

}