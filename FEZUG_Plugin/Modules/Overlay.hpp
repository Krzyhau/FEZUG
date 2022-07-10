#pragma once

#include "Utils/Common.hpp"
#include "Utils/Math.hpp"

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#define OVERLAY_FONTS_COUNT 2

class Overlay {
private:
    static DWORD SwapBufferOriginal;
    static void SwapBufferTrampoline();
    static void SwapBufferHook(_In_ HDC hDc);
    
    void(*frameCallback)() = nullptr;
public:
    void SetFrameCallback(void(*callback)());
    void Init();
    void HookSwapBuffer();
    void Draw();

public:
    struct Color {
        byte r, g, b, a;
        Color() :r(0), g(0), b(0), a(255) {};
        Color(byte r, byte g, byte b) : r(r), g(g), b(b), a(255) {};
        Color(byte r, byte g, byte b, byte a) : Color(r, g, b) { this->a = a; };
    };

    class Font {
    private:
        int height;
        int weight;
        unsigned int base = 0;
        bool built = false;
        const char* fontName;
    public:
        Font(const char* fontName, int height, int weight = 500);

        int GetHeight() { return height; }
        void Build();
        void Print(float x, float y, Color color, const char* text);
    };

private:
    Font *fonts[OVERLAY_FONTS_COUNT];
public:
    Vector2 GetWindowSize();
    void DrawFontText(int font, float x, float y, Color color, const char* format, ...);
    void FillRectangle(int x, int y, int width, int height, Color color);
    void DrawLine(int x1, int y1, int x2, int y2, Color color);
};



extern Overlay* gOverlay;
