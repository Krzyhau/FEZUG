#pragma once

#include "Utils/Common.hpp"

#define FEZUG_VERSION "FEZUG v0.1"

namespace FEZUG {
    static bool initiated;

    void Init(HMODULE hModule);
    void LateInit();
    void Frame();
    static HINSTANCE hInstance;
}