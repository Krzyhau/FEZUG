
#include "Overlay.hpp"

#include "FEZUG.hpp"
#include "Utils/Memory.hpp"
#include "Utils/Math.hpp"
#include "Utils/Logger.hpp"
#include "Modules/Input.hpp"
#include "Features/Console/Console.hpp"
#include "Modules/FEZEngine.hpp"


Overlay* gOverlay = new Overlay();


DWORD Overlay::SwapBufferOriginal = 0;

void __declspec(naked) Overlay::SwapBufferTrampoline() {
    __asm {
        PUSHFD                         //Stores EFLAGS
        PUSHAD                         //Stores GP Registers
        CALL SwapBufferHook           //Redirects the execution to our function
        POPAD                          //Restores GP Registers
        POPFD                          //Restores EFLAGS
        PUSH EBP                       //Restores Overwritten PUSH EBP
        MOV EBP, ESP                   //Restores Overwritten MOV  EBP, ESP
        JMP[SwapBufferOriginal] //Restores the execution to the original function
    }
}


void Overlay::HookSwapBuffer() {
    HMODULE hOpenGL = GetModuleHandle("opengl32.dll");
    assert(hOpenGL != 0);

    DWORD swapBufferHookAddress = (DWORD)GetProcAddress(hOpenGL, "wglSwapBuffers");
    SwapBufferOriginal = Memory::HookFunction(swapBufferHookAddress, (DWORD)SwapBufferTrampoline, 5);
}


void Overlay::SwapBufferHook(_In_ HDC hDc) {

    if (gOverlay->frameCallback != nullptr) {
        gOverlay->frameCallback();
    }

    // get GL functions that are not exported by the library
    auto glUseProgram = (void(WINAPI*)(GLuint))wglGetProcAddress("glUseProgram");
    auto glBlendFuncSeparate = (void(WINAPI*)(GLenum, GLenum, GLenum, GLenum))wglGetProcAddress("glBlendFuncSeparate");


    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    // resetting current shader program to the fixed pipeline and store it so we can reset it later
    GLint currentProgram = 0;
    glGetIntegerv(0x8B8D, &currentProgram);
    glUseProgram(0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport[2], viewport[3], 0, -1.000000, 550.000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // a shit ton of GL calls to make stuff appear. not sure if half of them is even needed lol.
    glDepthMask(false);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glStencilFunc(GL_ALWAYS, 0, 0x7FFFFFFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(true, true, true, true);
    glDisable(GL_CULL_FACE);

    gOverlay->Draw();

    glPopMatrix();
    glPopAttrib();

    glUseProgram(currentProgram);
}

void Overlay::Init() {
    HookSwapBuffer();

    fonts[0] = new Font("Lucida Console", 30, 700);
    fonts[1] = new Font("Lucida Console", 20);
}

void Overlay::SetFrameCallback(void(*callback)()) {
    frameCallback = callback;
}


void Overlay::Draw() {
    Vector2 wSize = GetWindowSize();

    /*if (!gInput->IsGameInputEnabled()) {
        DrawFontText(0, 0, wSize.y - 18, Color(255, 255, 255, 128), "Keyboard disabled!!");
    }*/

    if (!FEZEngine::GetGame()->IsValid()) return;

    for (Feature* feature : Feature::GetList()) {
        feature->Draw();
    }
}


Overlay::Font::Font(const char* fontName, int height, int weight) {
    this->fontName = fontName;
    this->height = height;
    this->weight = weight;
}

void Overlay::Font::Build() {
    HDC hdc = wglGetCurrentDC();
    base = glGenLists(256);
    HFONT hFont = CreateFontA(-(height), 0, 0, 0, weight, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    wglUseFontBitmaps(hdc, 0, 255, base);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    built = true;
}

void Overlay::Font::Print(float x, float y, Color color, const char* text) {
    if (!built) Build();

    glEnable(GL_LINE_SMOOTH);

    glColor4ub(color.r, color.g, color.b, color.a);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRasterPos2f(0, 0);

    glPushAttrib(GL_LIST_BIT);
    glListBase(base);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();

    glPopMatrix();
}




Vector2 Overlay::GetWindowSize() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return Vector2(viewport[2], viewport[3]);
}

void Overlay::DrawFontText(int font, float x, float y, Color color, const char* format, ...) {
    assert(font >= 0 && font < OVERLAY_FONTS_COUNT);

    char text[1024];
    va_list args;
    va_start(args, format);
    vsprintf_s(text, 1024, format, args);
    va_end(args);

    fonts[font]->Print(x, y, color, text);
}

void Overlay::FillRectangle(int x, int y, int width, int height, Color color) {
    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
    glVertex2i(x,y);
    glVertex2i(x,y+height);
    glVertex2i(x+width,y+height);
    glVertex2i(x+width,y);
    glEnd();
}

void Overlay::DrawLine(int x1, int y1, int x2, int y2, Color color) {
    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}