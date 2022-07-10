#pragma once

#include "Object.hpp"
#include "ActionType.hpp"
#include "TrileInstance.hpp"

#include "Utils/Math.hpp"

namespace FEZEngine {

    class PlayerManager : public Object {
    public:
        PlayerManager(void* ptr) : Object(ptr) {}
        FEZOBJ_OBJECT(TrileInstance, ForcedTreasure, 0x38)
        FEZOBJ_PROPERTY(ActionType, Action, 0x70)
        FEZOBJ_PROPERTY(Vector3, Position, 0xB8)
        FEZOBJ_PROPERTY(Vector3, Velocity, 0xD8)
    };

}