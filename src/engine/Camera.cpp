#include "Camera.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glfw3.h"

#define MOUSE_SENS 0.025f // TODO: make this configurable

glm::vec3 g_upVector( 0.0f, 1.0f, 0.0f );

/*
* TODO: arbitrarily set where the camera is looking
*/

Camera::Camera()
:   m_orientation( 1.0f, 0.0f, 0.0f, 0.0f ),
    m_cameraFront( 0.0f, 0.0f, -1.0f ),
    m_cameraPos( 0.0f, 0.0f, 0.0f )
{}

void Camera::NotifyMouseMovement( glm::vec2 const &mouseDelta )
{
    float yawChange = glm::radians( -mouseDelta.x * MOUSE_SENS );
    float pitchChange = glm::radians( mouseDelta.y * MOUSE_SENS );

    glm::vec3 cameraRight = m_orientation * glm::vec3( 1.0f, 0.0f, 0.0f );

    glm::quat yawRotation = glm::angleAxis( yawChange, g_upVector );

    glm::quat pitchRotation = glm::angleAxis( pitchChange, cameraRight );

    m_orientation = glm::normalize( pitchRotation * yawRotation * m_orientation );

    m_cameraFront = glm::normalize( m_orientation * glm::vec3( 0.0f, 0.0f, -1.0f ) );
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

glm::vec3 Camera::GetCameraPosition() const
{
    return m_cameraPos;
}

glm::vec3 Camera::GetCameraFront() const
{
    return m_cameraFront;
}
