#pragma once

struct GLFWwindow;

class OpenGLRenderer
{
public:
    void Initialise();
    void Update();

    bool WindowShouldClose() const;

    static OpenGLRenderer* GetInstance();

protected:
    OpenGLRenderer();

protected:
    GLFWwindow* m_window;
};