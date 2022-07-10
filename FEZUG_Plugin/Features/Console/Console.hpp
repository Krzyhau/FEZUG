#pragma once

#include "Modules/Overlay.hpp"
#include "Utils/Common.hpp"
#include "Features/Feature.hpp"

class Console : public Feature{
public:
    Console() : Feature() {};
    void Update();
    void Draw();
public:
    struct LogLine {
        string text;
        Overlay::Color color = Overlay::Color(255,255,255);
    };
private:
    bool enabled = false;
    float positionInterp = 0;

    string typingField;
    std::list<LogLine> logField;
public:
    inline void Enable() { enabled = true; }
    inline void Disable() { enabled = false; }
    inline void Toggle() { enabled = !enabled; }
    inline bool IsEnabled() const { return enabled; }

    void ProcessTypingField();

    void Clear();
    void WriteLine(const char* format, ...);
};

extern Console* gConsole;