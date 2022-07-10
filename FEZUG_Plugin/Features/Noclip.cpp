#include "Noclip.hpp"
#include "Modules/FEZEngine.hpp"

#include "Console/Command.hpp"
#include "Console/Console.hpp"

Noclip* gNoclip = new Noclip();

void Noclip::Update() {
    if (!enabled) return;

    Vector2 movement = FEZEngine::GetInputManager()->GetMovement();

    // reset velocity
    FEZEngine::GetPlayerManager()->SetVelocity(Vector3());
    // reset state
    FEZEngine::GetPlayerManager()->SetAction(FEZEngine::ActionType::Falling);

    // manipulate position directly
    auto pos = FEZEngine::GetPlayerManager()->GetPosition();

    pos += movement * speed;

    FEZEngine::GetPlayerManager()->SetPosition(pos);
}

void Noclip::Draw() {

}

CREATE_COMMAND_SIMPLE(noclip, "Enables or disables noclip") {
    gNoclip->enabled = !gNoclip->enabled;
    gConsole->WriteLine("Noclip has been %s!", gNoclip->enabled ? "enabled" : "disabled");
}