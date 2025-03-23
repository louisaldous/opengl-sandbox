#pragma once

#include "glm/glm.hpp"
#include "glfw3.h"

#include <array>

class InputManager
{
public:
    void UpdateCursorPosition( glm::vec2 const &newPosition);

    void NotifyKeyPressed( int key );
    void NotifyKeyReleased( int key );

    bool KeyPressed( int key ) const;

    glm::vec2 GetMouseDelta() const;

    static void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );

    static InputManager *GetInstance();

protected:
    InputManager();

    std::array<bool, GLFW_KEY_LAST> m_pressed;

    glm::vec2   m_mouseDelta;
    glm::vec2   m_lastMousePos;
};

