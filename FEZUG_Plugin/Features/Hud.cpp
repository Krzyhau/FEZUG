#include "Hud.hpp"
#include "Modules/FEZEngine.hpp"

Hud* gHud = new Hud();

void Hud::Update() {

}

void Hud::Draw() {
    Vector2 wSize = gOverlay->GetWindowSize();

    gOverlay->FillRectangle(4, wSize.y - 4 - 160, 700, 155, Overlay::Color(0, 0, 0, 200));

    Vector3 position = FEZEngine::GetPlayerManager()->GetPosition();
    gOverlay->DrawFontText(0, 6, wSize.y - 130, Overlay::Color(255, 255, 255, 255), "Gomez pos: %.03f %.03f %.03f", position.x, position.y, position.z);

    Vector3 velocity = FEZEngine::GetPlayerManager()->GetVelocity();
    // goddamn velocity is getting crazy when on ground. Just round it to 0.
    if (fabsf(velocity.y) < 0.0001) velocity.y = 0;
    gOverlay->DrawFontText(0, 6, wSize.y - 95, Overlay::Color(255, 255, 255, 255), "Gomez vel: %.04f %.04f %.04f", velocity.x, velocity.y, velocity.z);

    string gomezState = FEZEngine::ActionTypeToString(FEZEngine::GetPlayerManager()->GetAction(), true);
    gOverlay->DrawFontText(0, 6, wSize.y - 60, Overlay::Color(255, 255, 255, 255), "Gomez state: %s", gomezState.c_str());

    Vector3 cam = FEZEngine::GetCameraManager()->GetSideVector();
    gOverlay->DrawFontText(0, 6, wSize.y - 25, Overlay::Color(255, 255, 255, 255), "Cam side vec: %.03f %.03f %.03f", cam.x, cam.y, cam.z);
}

