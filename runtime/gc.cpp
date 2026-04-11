//
// Created by meian on 2026/4/10.
//

#include "gc.hpp"

using namespace lmx::runtime;

GCObject::GCObject(Object *obj) noexcept : obj(obj) {}

GCObject *GCObject::get() noexcept {
    rc++;
    return this;
}

void GCObject::release() noexcept {
    --rc;
}

void GCObject::mark() noexcept {
    is_marked = true;
}

GCObject::~GCObject() noexcept {
    release();
    if (obj) obj->~Object();
}

GC::GC() noexcept = default;

GC::~GC() noexcept = default;

GCObject *GC::new_obj(Object *obj) noexcept {
    objs.emplace_back(obj);
    return objs.back().get();
}
