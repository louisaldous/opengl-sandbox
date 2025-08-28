#version 330 core

struct Material
{
    sampler2D   m_diffuse;
    sampler2D   m_specular;
    float       m_shininess;
};

struct Light 
{
    vec3    m_ambient;
    vec3    m_diffuse;
    vec3    m_specular;

    vec3    m_position;
};

out vec4 FragColour;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    //
    // Ambient

    vec3 ambient = light.m_ambient * vec3(texture(material.m_diffuse, TexCoord));


    //
    // Diffuse

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.m_position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.m_diffuse * (diff * vec3(texture(material.m_diffuse, TexCoord)));


    //
    // Specular

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.m_shininess);
    vec3 specular = light.m_specular * (spec * vec3(texture(material.m_specular, TexCoord)));

    vec3 result = ambient + diffuse + specular;
    FragColour = vec4(result, 1.0);
}

