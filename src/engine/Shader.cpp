#include "Shader.h"

#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <cassert>

ShaderProgram::ShaderProgram( const char *vertexPath, const char *fragmentPath )
{
    std::ifstream vertexFStream;
    std::stringstream vertexStream;
    vertexFStream.open( vertexPath );
    if( vertexFStream.fail() )
    {
        std::stringstream message;
        message << "ERROR: failed to open " << vertexPath << std::endl;
        throw ShaderCompilationFailedException( message.str() );
    }
    vertexStream << vertexFStream.rdbuf();
    vertexFStream.close();

    std::ifstream fragmentFStream;
    std::stringstream fragmentStream;
    fragmentFStream.open( fragmentPath );
    if( fragmentFStream.fail() )
    {
        std::stringstream message;
        message << "ERROR: failed to open " << fragmentPath << std::endl;
        throw ShaderCompilationFailedException( message.str() );
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
    glGetProgramiv( m_id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog( m_id, 512, NULL, infoLog );
        std::stringstream message;
        message << "Shader program linking failed for " << vertexPath << "(vertex) and " << fragmentPath << "(fragment)" << std::endl;
        message << "\"" << infoLog << "\"" << std::endl;
        throw ShaderCompilationFailedException( message.str() );
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_id );
}

void ShaderProgram::Use() const
{
    glUseProgram( m_id );
}

void ShaderProgram::Set( const std::string &uniformName, const glm::mat4 &data ) const
{
    GLint loc = glGetUniformLocation( m_id, uniformName.c_str() );
    glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr( data ) );
}

void ShaderProgram::Set( const std::string &uniformName, int value ) const
{
    GLint loc = glGetUniformLocation( m_id, uniformName.c_str() );
    glUniform1i( loc, value );
}

std::unique_ptr<ShaderProgram> ShaderProgram::Create( const char *vertexPath, const char *fragmentPath )
{
    return std::unique_ptr<ShaderProgram>( new ShaderProgram( vertexPath, fragmentPath ) );
}

uint32_t ShaderProgram::CreateShader( uint32_t type, const char *source ) const
{
    uint32_t shaderID = glCreateShader( type );
    glShaderSource( shaderID, 1, &source, NULL );
    glCompileShader( shaderID );

    int32_t success;
    char infoLog[512];
    glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );

    if( !success )
    {
        glGetShaderInfoLog( shaderID, 512, NULL, infoLog );

        std::stringstream message;
        message << "Shader compilation failed for " << source << std::endl;
        message << "\"" << infoLog << "\"" << std::endl;
        throw ShaderCompilationFailedException( message.str() );
    }

    return shaderID;
}

ShaderCompilationFailedException::ShaderCompilationFailedException( std::string &&message )
    : m_message( std::move( message ) )
{
}

const char *ShaderCompilationFailedException::what() const noexcept
{
    return m_message.c_str();
}
