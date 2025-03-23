#include "InputManager.h"

InputManager::InputManager()
:   m_lastMousePos( 0, 0 ),
    m_mouseDelta( 0, 0 )
{
    m_pressed.fill( false );
}

InputManager* InputManager::GetInstance()
{
    static InputManager inputManager;
    return &inputManager;
}

void InputManager::UpdateCursorPosition( glm::vec2 const &newPosition )
{
    m_mouseDelta = { newPosition.x - m_lastMousePos.x, m_lastMousePos.y - newPosition.y };
    m_lastMousePos = newPosition;
}

void InputManager::NotifyKeyPressed( int key )
{
    if( key < 0 || key >= GLFW_KEY_LAST ) return;
    m_pressed[key] = true;
}

void InputManager::NotifyKeyReleased( int key )
{
    if( key < 0 || key >= GLFW_KEY_LAST ) return;
    m_pressed[key] = false;
}

bool InputManager::KeyPressed( int key ) const
{
    if( key < 0 || key >= GLFW_KEY_LAST ) return false;
    return m_pressed[key];
}

glm::vec2 InputManager::GetMouseDelta() const
{
    return m_mouseDelta;
}

void InputManager::KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    InputManager* inputManager = GetInstance();

    if( action == GLFW_PRESS )
    {
        inputManager->NotifyKeyPressed( key );
    }
    else if( action == GLFW_RELEASE )
    {
        inputManager->NotifyKeyReleased( key );
    }
}


