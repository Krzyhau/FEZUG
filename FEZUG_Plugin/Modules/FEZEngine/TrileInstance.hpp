#pragma once

#include "Object.hpp"

namespace FEZEngine {

    class TrileInstance : public Object {
    public:
        TrileInstance(void* ptr) : Object(ptr) {}
    };

}