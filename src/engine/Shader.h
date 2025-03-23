#pragma once

#include <cstdint>
#include <exception>
#include <string>
#include <memory>

#include "glm/matrix.hpp"

class ShaderCompilationFailedException : public std::exception
{
public:
    ShaderCompilationFailedException( std::string &&message );
    const char *what() const noexcept override;

protected:
    std::string m_message;
};

class ShaderProgram
{
    // TODO: strong exception guarantee
public:
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram();

    void Use() const;
    void Set( const std::string &uniformName, const glm::mat4 &data ) const;
    void Set( const std::string &uniformName, int value ) const;

    static std::unique_ptr<ShaderProgram> Create( const char *vertexPath, const char *fragmentPath );

private:
    ShaderProgram( const char *vertexPath, const char *fragmentPath );

    uint32_t CreateShader( uint32_t type, const char *source ) const;

protected:
    uint32_t m_id;
};

