#pragma once

#include "Modules/Overlay.hpp"
#include "Utils/Common.hpp"
#include "Features/Feature.hpp"

class Noclip : public Feature {
public:
    bool enabled = false;
    float speed = 0.1f;
public:
    Noclip() : Feature() {};
    void Update();
    void Draw();
};

extern Noclip* gNoclip;