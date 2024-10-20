#ifndef shader_h
#define shader_h

#include <cstdint>

class Shader
{
public:
    uint32_t m_id;

public:
    Shader( const char *vertexPath, const char *fragmentPath );

    void Use() const;

protected:
    uint32_t CreateShader( uint32_t type, const char *source ) const;
};

#endif // !Shader_h

