#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
    Camera();

    void NotifyMouseMovement( glm::vec2 const &mouseDelta );

    void SetCameraFront( glm::vec3 &front );
    void SetCameraPosition( glm::vec3 &pos );

    glm::vec3 GetCameraPosition() const;
    glm::vec3 GetCameraFront() const;
    glm::mat4 GetViewMatrix() const;

    static Camera *GetInstance();

protected:
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::quat m_orientation;
};

extern glm::vec3 g_upVector;
