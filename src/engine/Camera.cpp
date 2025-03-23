#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glfw3.h"

#define MOUSE_SENS 0.025f // TODO: make this configurable

glm::vec3 g_upVector( 0.0f, 1.0f, 0.0f );

Camera::Camera()
:   m_pitch( 0.0f ),
    m_yaw( 0.0f ),
    m_cameraFront( 0.0f, 0.0f, -1.0f ),
    m_cameraPos( 0.0f, 0.0f, 0.0f )
{}

void Camera::NotifyMouseMovement( glm::vec2 const &mouseDelta )
{
    m_yaw += (float)mouseDelta.x * MOUSE_SENS;
    m_pitch += (float)mouseDelta.y * MOUSE_SENS;

    if( m_pitch > 89 ) m_pitch = 89;
    if( m_pitch < -89 ) m_pitch = -89;

    glm::vec3 direction( cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) ),
                         sin( glm::radians( m_pitch ) ),
                         sin( glm::radians( m_yaw ) * cos( glm::radians( m_pitch ) ) ) );

    m_cameraFront = glm::normalize( direction );
}

void Camera::SetCameraFront( glm::vec3 &front )
{
    m_cameraFront = front;
}

void Camera::SetCameraPosition( glm::vec3 &pos )
{
    m_cameraPos = pos;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt( m_cameraPos, m_cameraPos + m_cameraFront, g_upVector );
}

glm::vec3 Camera::GetCameraFront() const
{
    return m_cameraFront;
}
