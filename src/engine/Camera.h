#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    void NotifyMouseMovement( glm::vec2 const &mouseDelta );

    void SetCameraFront( glm::vec3 &front );
    void SetCameraPosition( glm::vec3 &pos );

    glm::vec3 GetCameraFront() const;
    glm::mat4 GetViewMatrix() const;

    static Camera *GetInstance();

protected:
    float m_pitch;
    float m_yaw;

    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
};

extern glm::vec3 g_upVector;
