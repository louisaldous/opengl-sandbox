#pragma once

#include "glm/glm.hpp"

#include "engine/Camera.h"

class Player
{
public:
    Player();

    void Update();

    const Camera &GetCamera() const;

protected:
    Camera    m_camera;
    glm::vec3 m_pos;
};

