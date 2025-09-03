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


// TODO: remove this macro in favour of a packaging system
#ifndef DATA_DIRECTORY
#error "DATA_DIRECTORY is not defined. Please define it to specify where data is stored."
#endif

#define PI 3.1415f
#define DATA(x) DATA_DIRECTORY + std::string(x) // TODO: remove me, temp macro for convenience

static glm::vec3 s_lightPos( 1.2f, 1.0f, 2.0f );
static glm::vec3 s_lightColour( 1.0f, 1.0f, 1.0f );

float s_cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static uint32_t LoadTexture( std::string const &path, int textureUnit )
{
    uint32_t textureId;
    glGenTextures( 1, &textureId );
    glActiveTexture( GL_TEXTURE0 + textureUnit );
    glBindTexture( GL_TEXTURE_2D, textureId );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f );

    int width, height, channels;
    unsigned char *data = stbi_load( path.c_str(), &width, &height, &channels, 0 );

    if( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }

    stbi_image_free( data );

    return textureId;
}

int main()
{
    OpenGLRenderer* renderer = OpenGLRenderer::GetInstance();
    renderer->Initialise();

    //
    // Set up buffers

    uint32_t cubeVAO, VBO;
    glGenVertexArrays( 1, &cubeVAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( cubeVAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( s_cubeVertices ), s_cubeVertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void *)(3 * sizeof( float )) );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void *)(6 * sizeof( float )) );
    glEnableVertexAttribArray( 2 );

    uint32_t lightVAO;
    glGenVertexArrays( 1, &lightVAO );
    glBindVertexArray( lightVAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );

    LoadTexture( DATA( "textures/container2.png" ), 0 );
    LoadTexture( DATA( "textures/container2_specular.png" ), 1 );

    std::unique_ptr<ShaderProgram> lightingShader;
    std::unique_ptr<ShaderProgram> lightSourceShader;
    try
    {
        lightingShader = std::make_unique<ShaderProgram>( DATA( "shaders/cubeVert.glsl" ), DATA( "shaders/cubeFrag.glsl" ) );
        lightSourceShader = std::make_unique<ShaderProgram>( DATA( "shaders/lightVert.glsl" ), DATA( "shaders/lightFrag.glsl" ) );
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

    glm::mat4 projection = glm::perspective( glm::radians( 60.0f ), 4.0f / 3.0f, 0.1f, 100.0f );

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
        model = glm::translate( model, s_lightPos );
        model = glm::scale(model, glm::vec3(0.2f)); 

        lightSourceShader->Use();
        lightSourceShader->SetMat4( "model", model );
        lightSourceShader->SetMat4( "projection", projection );
        lightSourceShader->SetMat4( "view", view );
        lightSourceShader->SetVec3( "lightColour", s_lightColour );

        glBindVertexArray( lightVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );


        //
        // normal cube

        model = glm::mat4( 1.0f );

        lightingShader->Use();
        lightingShader->SetMat4( "model", model );
        lightingShader->SetMat4( "projection", projection );
        lightingShader->SetMat4( "view", view );

        lightingShader->SetInt32( "material.m_diffuse", 0 );
        lightingShader->SetInt32( "material.m_specular", 1 );
        lightingShader->SetFloat( "material.m_shininess", 32.0f );

        lightingShader->SetVec3( "light.m_ambient", glm::vec3( 0.2f, 0.2f, 0.2f ) );
        lightingShader->SetVec3( "light.m_diffuse", s_lightColour );
        lightingShader->SetVec3( "light.m_specular", glm::vec3( 1.0f, 1.0f, 1.0f ) );
        lightingShader->SetVec3( "light.m_position", s_lightPos );

        lightingShader->SetVec3( "viewPos", camera.GetCameraPosition() );

        glBindVertexArray( cubeVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

        if( InputManager::GetInstance()->KeyPressed( GLFW_KEY_ESCAPE ) ) break;
        renderer->Update();
    }

    glfwTerminate();
    return 0;
}