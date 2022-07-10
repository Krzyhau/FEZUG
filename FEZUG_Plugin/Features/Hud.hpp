#pragma once

#include "Modules/Overlay.hpp"
#include "Utils/Common.hpp"
#include "Features/Feature.hpp"

class Hud : public Feature {
public:
    Hud() : Feature() {};
    void Update();
    void Draw();
};

extern Hud* gHud;