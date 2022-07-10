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
    FEZEngine::GetPlayerManager()->SetAction(FEZEngine::ActionType::Idle);

    // manipulate position directly
    auto pos = FEZEngine::GetPlayerManager()->GetPosition();
    if ((oldPos - pos).Length() < 0.1f)pos = oldPos;

    pos += FEZEngine::GetCameraManager()->GetSideVector() * movement.x * speed;
    pos += Vector3(0, movement.y, 0) * speed;

    oldPos = pos;
    FEZEngine::GetPlayerManager()->SetPosition(pos);
}

void Noclip::Draw() {

}

CREATE_COMMAND_SIMPLE(noclip, "Enables or disables noclip") {
    gNoclip->enabled = !gNoclip->enabled;
    gConsole->WriteLine("Noclip has been %s!", gNoclip->enabled ? "enabled" : "disabled");
}

CREATE_COMMAND_PARAMS(noclip_speed, "Sets speed of the noclip.", 1) {
    float speed = std::stof(params[0]);
    gNoclip->speed = speed;
}