#pragma once

#include "Object.hpp"
#include "Utils/Math.hpp"

namespace FEZEngine {

    enum ButtonState {
        Up,
        Pressed,
        Released,
        Down
    };

    struct ButtonStates {
        ButtonState Jump;
        ButtonState Back;
        ButtonState OpenInventory;
        ButtonState Start;
        ButtonState RotateLeft;
        ButtonState RotateRight;
        ButtonState CancelTalk;
        ButtonState Up;
        ButtonState Down;
        ButtonState Left;
        ButtonState Right;
        ButtonState ClampLook;
        ButtonState FpsToggle;
        ButtonState ExactUp;
        ButtonState MapZoomIn;
        ButtonState MapZoomOut;
    };

    class InputManager : public Object {
    public:
        InputManager(void* ptr) : Object(ptr) {}

        FEZOBJ_PROPERTY(bool, EnabledState, 0x18)
        FEZOBJ_PROPERTY(bool, MouseEnabledState, 0x19)
        FEZOBJ_PROPERTY(bool, KeyboardEnabledState, 0x1A)
        FEZOBJ_PROPERTY(bool, ControllerEnabledState, 0x1B)

        FEZOBJ_PROPERTY(ButtonStates, ButtonStates, 0x3C)

        FEZOBJ_PROPERTY(Vector2, Movement, 0x88)
        FEZOBJ_PROPERTY(Vector2, FreeLook, 0x90)

        
    };

}