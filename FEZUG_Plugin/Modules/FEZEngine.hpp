#pragma once

#include "Utils/Common.hpp"
#include "FEZEngine/Game.hpp"

#define FEZOBJ_STATIC_DECL(type) type*& Get##type##();
#define FEZOBJ_STATIC_REDECL(type) type*& Get##type##(){ static type* gp##type; return gp##type;}
#define FEZOBJ_STATIC_CREATE(type, val) Get##type##() = new type((void*)val);
#define FEZOBJ_STATIC_SET(type, val) Get##type##() = val;

namespace FEZEngine {
    FEZOBJ_STATIC_DECL(Game);
    FEZOBJ_STATIC_DECL(LevelManager);
    FEZOBJ_STATIC_DECL(PlayerManager);
    FEZOBJ_STATIC_DECL(InputManager);
    FEZOBJ_STATIC_DECL(CameraManager);
    void Init();
    void UpdateStaticObjects();
    void Update();

    uintptr_t GetFuncAddr(int offset);
}
