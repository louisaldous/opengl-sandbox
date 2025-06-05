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

static float s_cubeVertices [] = {
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

int main()
{
    OpenGLRenderer* renderer = OpenGLRenderer::GetInstance();
    renderer->Initialise();

    //
    // Create Shader Program

    std::unique_ptr<ShaderProgram> shaderProgram;
    try
    {
        shaderProgram = ShaderProgram::Create( "data/shader.vs", "data/shader.fs" );
    }
    catch( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    glm::vec3 cubePositions [] = {
        glm::vec3( 0.0f,  0.0f,  0.0f ),
        glm::vec3( 2.0f,  5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3( -1.7f,  3.0f, -7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f,  2.0f, -2.5f ),
        glm::vec3( 1.5f,  0.2f, -1.5f ),
        glm::vec3( -1.3f,  1.0f, -1.5f )
    };


    //
    // Set up buffers

    uint32_t VAO, VBO, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

    glBufferData( GL_ARRAY_BUFFER, sizeof( s_cubeVertices ), s_cubeVertices, GL_STATIC_DRAW );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );

    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)(3 * sizeof(float)) );
    glEnableVertexAttribArray( 1 );


    //
    // Set up textures

    uint32_t textures[2] = { 0 };
    glGenTextures( 2, textures );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textures[0] );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32_t width, height, nrChannels;
    unsigned char *data = stbi_load( "data/brick.jpg", &width, &height, &nrChannels, 0 );
    assert( data );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
    stbi_image_free( data );

    glActiveTexture( GL_TEXTURE0 + 1 );
    glBindTexture( GL_TEXTURE_2D, textures[1] );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load( "data/container.jpg", &width, &height, &nrChannels, 0 );
    assert( data );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
    stbi_image_free( data );

    glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

    //
    // Enable shaders

    shaderProgram->Use();
    shaderProgram->Set( "projection", projection );

    double lastTime = 0.0;
    double thisTime;

    Player player;
    const Camera &camera = player.GetCamera();

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
        shaderProgram->Set( "view", view );

        for( int32_t i = 0; i < 10; i++ )
        {
            shaderProgram->Set( "ourTexture", i % 2 );
            glm::mat4 model = glm::mat4( 1.0f );

            model = glm::translate( model, cubePositions[i] );
            
            float angle = 20.0f * i;
            model = glm::rotate( model, glm::radians( angle ), glm::vec3( 0.5f, 1.0f, 0.0f ) );

            shaderProgram->Set( "model", model );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }

        if( InputManager::GetInstance()->KeyPressed( GLFW_KEY_ESCAPE ) ) break;
        renderer->Update();
    }

    glfwTerminate();
    return 0;
}