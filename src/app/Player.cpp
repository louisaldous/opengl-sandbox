#include "Player.h"

#include <glfw3.h>

#include "engine/InputManager.h"

#include "app/App.h"

#define MOVE_SPEED 3.0f

Player::Player()
    : m_pos( 0.0f, 0.0f, 3.0f )
{}

void Player::Update()
{
    InputManager* inputManager = InputManager::GetInstance();

    float toMove = (float)g_deltaTime * MOVE_SPEED;
    if( inputManager->KeyPressed( GLFW_KEY_LEFT_SHIFT ) ) toMove *= 2;

    if( inputManager->KeyPressed( GLFW_KEY_W ) )
    {
        m_pos += m_camera.GetCameraFront() * toMove;
    }

    if( inputManager->KeyPressed( GLFW_KEY_A ) )
    {
        m_pos -= glm::normalize( glm::cross( m_camera.GetCameraFront(), g_upVector ) ) * toMove;
    }

    if( inputManager->KeyPressed( GLFW_KEY_S ) )
    {
        m_pos -= m_camera.GetCameraFront() * toMove;
    }

    if( inputManager->KeyPressed( GLFW_KEY_D ) )
    {
        m_pos += glm::normalize( glm::cross( m_camera.GetCameraFront(), g_upVector ) ) * toMove;
    }

    m_camera.SetCameraPosition( m_pos );

    glm::vec2 mouseDelta = inputManager->GetMouseDelta();
    m_camera.NotifyMouseMovement( mouseDelta );
}

const Camera &Player::GetCamera() const
{
    return m_camera;
}
