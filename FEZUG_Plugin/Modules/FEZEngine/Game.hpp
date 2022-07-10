#pragma once

#include "Object.hpp"
#include "LevelManager.hpp"
#include "InputManager.hpp"
#include "CameraManager.hpp"

namespace FEZEngine {

    class Game : public Object {
    public:
        Game(void* ptr) : Object(ptr) {}
        FEZOBJ_OBJECT_GETTER(InputManager, InputManager, 0x74)
        FEZOBJ_OBJECT_GETTER(LevelManager, LevelManager, 0x7C)
        FEZOBJ_OBJECT_GETTER(CameraManager, CameraManager, 0x84)
    };

}