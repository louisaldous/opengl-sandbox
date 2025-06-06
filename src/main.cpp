#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "stb_image.h"

#include "engine/Camera.h"
#include "engine/InputManager.h"
#include "engine/Renderer.h"
#include "engine/Shader.h"

#include "app/App.h"
#include "app/Player.h"

#include <memory>
#include <iostream>

#define PI 3.1415f

static constexpr float s_cubeVertices [] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static constexpr glm::vec3 lightPos( 1.2f, 1.0f, 2.0f );

int main()
{
    OpenGLRenderer* renderer = OpenGLRenderer::GetInstance();
    renderer->Initialise();

    //
    // Set up buffers

    uint32_t VAO, VBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( s_cubeVertices ), s_cubeVertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );

    uint32_t lightVAO;
    glGenVertexArrays( 1, &lightVAO );
    glBindVertexArray( lightVAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );



    std::unique_ptr<ShaderProgram> lightingShader;
    std::unique_ptr<ShaderProgram> lightSourceShader;
    try
    {
        lightingShader = std::make_unique<ShaderProgram>( DATA_DIRECTORY + std::string( "lightingShaderVert.glsl" ), DATA_DIRECTORY + std::string( "lightingShaderFrag.glsl" ) );
        lightSourceShader = std::make_unique<ShaderProgram>( DATA_DIRECTORY + std::string( "lightSourceShaderVert.glsl" ), DATA_DIRECTORY + std::string( "lightSourceShaderFrag.glsl" ) );
    }
    catch( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    Player player;
    const Camera &camera = player.GetCamera();

    double lastTime = 0.0;
    double thisTime = 0.0;

    glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

    while( !renderer->WindowShouldClose() )
    {
        player.Update();

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glEnable( GL_DEPTH_TEST );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        thisTime = glfwGetTime();
        g_deltaTime = thisTime - lastTime;
        lastTime = thisTime;

        glm::mat4 view = camera.GetViewMatrix();

        //
        // light source cube

        glm::mat4 model = glm::mat4( 1.0f );
        model = glm::translate( model, lightPos );
        model = glm::scale(model, glm::vec3(0.2f)); 

        lightSourceShader->Use();
        lightSourceShader->Set( "model", model );
        lightSourceShader->Set( "projection", projection );
        lightSourceShader->Set( "view", view );
        glBindVertexArray( lightVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );


        //
        // normal cube

        model = glm::mat4( 1.0f );

        lightingShader->Use();
        lightingShader->Set( "model", model );
        lightingShader->Set( "projection", projection );
        lightingShader->Set( "view", view );
        lightingShader->Set( "objectColour", glm::vec3( 1.0f, 0.5f, 0.31f ) );
        lightingShader->Set( "lightColour", glm::vec3( 1.0f, 1.0f, 1.0f ) );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        if( InputManager::GetInstance()->KeyPressed( GLFW_KEY_ESCAPE ) ) break;
        renderer->Update();
    }

    glfwTerminate();
    return 0;
}