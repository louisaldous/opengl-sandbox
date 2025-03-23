#include "Renderer.h"

#include "glad/glad.h"

#include "engine/InputManager.h"

#include <iostream>

static void framebuffer_size_callback( GLFWwindow *window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void OpenGLRenderer::Initialise()
{
    if( m_window ) return;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow( 800, 600, "Main Window", nullptr, nullptr);
    if( !window )
    {
        glfwTerminate();
        std::cout << "Failed to create window!\n";
        return;
    }

    glfwMakeContextCurrent( window );
    
    if( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialise GLAD\n";
    }

    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetKeyCallback( window, InputManager::KeyCallback );

    m_window = window;
}

OpenGLRenderer* OpenGLRenderer::GetInstance()
{
    static OpenGLRenderer renderer;
    return &renderer;
}

void OpenGLRenderer::Update()
{
    double cursorX, cursorY;
    glfwGetCursorPos( m_window, &cursorX, &cursorY );
    InputManager *inputManager = InputManager::GetInstance();
    inputManager->UpdateCursorPosition( { cursorX, cursorY } );

    glfwSwapBuffers( m_window );
    glfwPollEvents();
}

bool OpenGLRenderer::WindowShouldClose() const
{
    return glfwWindowShouldClose( m_window );
}

OpenGLRenderer::OpenGLRenderer()
    : m_window( nullptr )
{
}
