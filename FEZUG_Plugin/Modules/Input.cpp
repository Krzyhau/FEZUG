#include "Input.hpp"

#include "Utils/Memory.hpp"
#include "Utils/Logger.hpp"

Input* gInput = new Input();


LRESULT __stdcall Input::KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT kbd = *((KBDLLHOOKSTRUCT*)lParam);

        if (wParam == WM_KEYFIRST || wParam == WM_SYSKEYDOWN)
        {
            //Logger::Log("Input Debug", "Pressed key: %d", kbd.vkCode);
            gInput->PressKey(kbd.vkCode);
        }
        if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            //Logger::Log("Input Debug", "Released key: %d", kbd.vkCode);
            gInput->ReleaseKey(kbd.vkCode);
        }
    }

    return CallNextHookEx(gInput->keyboardHook, nCode, wParam, lParam);
}


void Input::Init() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0);

    initialized = true;
}

void Input::Update() {

    // handle key states
    for (int i = 0; i < 256; i++) {
        // save a copy of states
        oldKeyStates[i] = keyStates[i];

        // fetch new states
        if (keyStatesPressBuffer[i]) {
            keyStates[i] = true;
            keyStatesPressBuffer[i] = false;
        }
        else if (keyStatesReleaseBuffer[i]) {
            // doesn't fetch release request if press request was done
            // so we can actually get the press when both actions happened
            // in the same frame (omega fast press)
            keyStates[i] = false;
            keyStatesReleaseBuffer[i] = false;
        }
    }

    // handle typing

    typeBuffer.clear();

    for (USHORT key : rawTypeBuffer) {

        if (key == VK_ESCAPE || key == VK_RETURN)continue;

        CHAR uc[3] = {'\0', '\0', '\0'};

        BYTE kb[256] = {0};
        bool err = GetKeyboardState(kb);
        HKL kbLayout = LoadKeyboardLayout("00000409", 0); // US keyboard. fuck diacritics
        UINT scanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        int charCount = ToAsciiEx(key, scanCode, kb, (LPWORD)uc, 0, kbLayout);

        if (charCount > 0) {
            int c = (int)uc[0];
            // typed character needs to be either alphanumerical or a backspace
            if (c >= 32 || c == 8) {
                typeBuffer += string(uc);
            }
        }
    }
    rawTypeBuffer.clear();
}

void Input::PressKey(USHORT key) {
    if (!keyStates[key]) {
        keyStatesPressBuffer[key] = true;
    }
    rawTypeBuffer.push_back(key);
}

void Input::ReleaseKey(USHORT key) {
    if (keyStates[key]) {
        keyStatesReleaseBuffer[key] = true;
    }
}