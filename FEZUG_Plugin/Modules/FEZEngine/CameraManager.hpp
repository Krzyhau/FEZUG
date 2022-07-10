#pragma once

#include "Object.hpp"
#include "ActionType.hpp"
#include "TrileInstance.hpp"

#include "Utils/Math.hpp"

namespace FEZEngine {

    class CameraManager : public Object {
    public:
        CameraManager(void* ptr) : Object(ptr) {}
        FEZOBJ_PROPERTY_GETTER(Vector3, SideVector, 0x24)
        FEZOBJ_PROPERTY_GETTER(Vector3, ForwardVector, 0x34)
    };

}