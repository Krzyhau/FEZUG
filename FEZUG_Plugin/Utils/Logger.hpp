#pragma once

#include "Utils/Common.hpp"

namespace Logger {
    bool Init();
    void Log(string component, string message, ...);
}