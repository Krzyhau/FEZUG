#pragma once

#include "Object.hpp"
#include "LevelData.hpp"
#include "PlayerManager.hpp"

namespace FEZEngine {

    class LevelManager : public Object {
    public:
        LevelManager(void* ptr) : Object(ptr) {}

        FEZOBJ_OBJECT_GETTER(LevelData, Level, 0x38)
        FEZOBJ_OBJECT_GETTER(PlayerManager, PlayerManager, 0x88)

        FEZOBJ_FUNC_0(0xDD14, void, Reset)
    };

}
