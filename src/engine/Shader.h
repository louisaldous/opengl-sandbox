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

    ShaderProgram( const std::string &vertexPath, const std::string &fragmentPath );
    ~ShaderProgram();

    void Use() const;
    void SetMat4( const std::string &uniformName, const glm::mat4 &data ) const;
    void SetVec3( const std::string &uniformName, const glm::vec3 &data ) const;
    void SetInt32( const std::string &uniformName, int value ) const;
    void SetFloat( const std::string &uniformName, float value ) const;

private:
    uint32_t CreateShader( uint32_t type, const char *source ) const;

protected:
    uint32_t m_id;
};

