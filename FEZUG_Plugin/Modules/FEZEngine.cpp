#include "FEZEngine.hpp"

#include "Utils/Memory.hpp"
#include "Utils/Logger.hpp"
#include "Features/Console/Command.hpp"
#include "Features/Console/Console.hpp"
#include "Input.hpp"

namespace FEZEngine {
    FEZOBJ_STATIC_REDECL(Game);
    FEZOBJ_STATIC_REDECL(LevelManager);
    FEZOBJ_STATIC_REDECL(PlayerManager);
    FEZOBJ_STATIC_REDECL(InputManager);

    void Init() {
        FEZOBJ_STATIC_CREATE(Game, (Memory::ScanInMemoryPages("01 01 00 00 00 40 0D 03") - 0x53));
        Logger::Log("FEZUG", "Game pointer: %08X", GetGame()->ThisPtr());

        UpdateStaticObjects();
        Logger::Log("FEZUG", "LevelManager pointer: %08X", GetGame()->GetLevelManager()->ThisPtr());
        Logger::Log("FEZUG", "PlayerManager pointer: %08X", GetGame()->GetLevelManager()->GetPlayerManager()->ThisPtr());
        Logger::Log("FEZUG", "state: %s", ActionTypeToString(GetPlayerManager()->GetAction(), true).c_str());

        //I "tried" to get a symbol for one of the functions but failed miserably
        /*HMODULE hFEZ = GetModuleHandle("FEZ.exe");
        if (!hFEZ)return;
        DWORD GameLevelManagerReset = (DWORD)GetProcAddress(hFEZ, "FezGame.Services.GameLevelManager::Reset_IL");
        Logger::Log("FEZUG", "GameLevelManagerReset pointer: %08X", GameLevelManagerReset);*/
    }

    void UpdateStaticObjects() {
        // make sure static pointers are up to date
        FEZOBJ_STATIC_SET(LevelManager, GetGame()->GetLevelManager());
        FEZOBJ_STATIC_SET(PlayerManager, GetLevelManager()->GetPlayerManager());
        FEZOBJ_STATIC_SET(InputManager, GetGame()->GetInputManager());
    }

    void Update() {
        UpdateStaticObjects();
        
        if (GetInputManager()->GetEnabledState() != gInput->IsGameInputEnabled()) {
            GetInputManager()->SetEnabledState(gInput->IsGameInputEnabled());
        }
    }
}

CREATE_COMMAND_PARAMS(setpos, "Sets Gomez's position", 3) {
    float x = std::stof(params[0]);
    float y = std::stof(params[1]);
    float z = std::stof(params[2]);
    FEZEngine::GetPlayerManager()->SetPosition(Vector3(x, y, z));
}
