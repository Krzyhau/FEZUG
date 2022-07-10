#pragma once

#include "Utils/Common.hpp"


class Input {
private:
    static LRESULT __stdcall KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
    HHOOK keyboardHook;

private:
    bool initialized = false;
    bool gameInputEnabled = true;

    bool keyStatesPressBuffer[256] = { false };
    bool keyStatesReleaseBuffer[256] = { false };
    bool keyStates[256] = { false };
    bool oldKeyStates[256] = { false };

    vector<USHORT> rawTypeBuffer;
    string typeBuffer;

public:
    inline bool IsInitialized() const { return initialized; };
    void Init();
    void Update();

    inline bool IsGameInputEnabled() const { return gameInputEnabled; }
    inline void ToggleGameInput() { gameInputEnabled = !gameInputEnabled; }
    inline void SetGameInputEnabled(bool state) { gameInputEnabled = state; }

    inline bool IsKeyPressed(USHORT key) const { return !oldKeyStates[key] && keyStates[key]; }
    inline bool IsKeyHeld(USHORT key) const { return keyStates[key]; }
    inline bool IsKeyReleased(USHORT key)const { return oldKeyStates[key] && !keyStates[key]; }
    void PressKey(USHORT key);
    void ReleaseKey(USHORT key);

    inline string GetTypeBuffer() const { return typeBuffer; }

};

extern Input* gInput;

