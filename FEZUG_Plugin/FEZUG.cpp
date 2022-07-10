#include "FEZUG.hpp"

#include "Utils/Common.hpp"
#include "Utils/Memory.hpp"
#include "Utils/Logger.hpp"
#include "Modules/Input.hpp"
#include "Modules/Overlay.hpp"
#include "Features/Feature.hpp"
#include "Modules/FEZEngine.hpp"


void FEZUG::Frame() {
    if (!initiated) LateInit();

    FEZEngine::Update();
    gInput->Update();

    for (Feature* feature : Feature::GetList()) {
        feature->Update();
    }
}

// prep of all necessary stuff for the plugin
void FEZUG::Init(HMODULE hModule) {
    hInstance = hModule;
    Logger::Init();

    gOverlay->Init();
    gOverlay->SetFrameCallback(FEZUG::Frame);
}

void FEZUG::LateInit() {
    gInput->Init();
    FEZEngine::Init();

    initiated = true;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FEZUG::Init, hModule, 0, 0);
        if (hThread != nullptr) {
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}