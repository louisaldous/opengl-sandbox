#include "Shader.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>

#include "glad/glad.h"

Shader::Shader( const char *vertexPath, const char *fragmentPath )
{
    std::ifstream vertexFStream, fragmentFStream;
    std::stringstream vertexStream, fragmentStream;

    vertexFStream.open( vertexPath );
    if( vertexFStream.fail() )
    {
        std::cerr << "ERROR: failed to open " << vertexPath << " for vertex shader\n";
        std::abort();
    }

    vertexStream << vertexFStream.rdbuf();
    vertexFStream.close();

    fragmentFStream.open( fragmentPath );
    if( fragmentFStream.fail() )
    {
        std::cerr << "ERROR: failed to open " << fragmentPath << " for fragment shader\n";
        std::abort();
    }

    fragmentStream << fragmentFStream.rdbuf();
    fragmentFStream.close();

    std::string vertexString = vertexStream.str();
    uint32_t vertexShader = CreateShader( GL_VERTEX_SHADER, vertexString.c_str() );

    std::string fragmentString = fragmentStream.str();
    uint32_t fragmentShader = CreateShader( GL_FRAGMENT_SHADER, fragmentString.c_str() );

    m_id = glCreateProgram();
    glAttachShader( m_id, vertexShader );
    glAttachShader( m_id, fragmentShader );
    glLinkProgram( m_id );

    int32_t success;
    char infoLog[512];
    glGetShaderiv( m_id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        std::abort();
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
}

void Shader::Use() const
{
    glUseProgram( m_id );
}

uint32_t Shader::CreateShader( uint32_t type, const char *source ) const
{
    uint32_t shaderID = glCreateShader( type );
    glShaderSource( shaderID, 1, &source, NULL );
    glCompileShader( shaderID );

    int32_t success;
    char infoLog[512];
    glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );

    if(!success)
    {
        glGetShaderInfoLog( shaderID, 512, NULL, infoLog );
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::abort();
    }

    return shaderID;
}
