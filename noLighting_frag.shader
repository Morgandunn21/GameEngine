#version 430

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec3 tint;
    float shininess;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

void main()
{
    FragColor = vec4(vec3(texture(material.diffuse, TexCoords)) * material.tint, 1.0); // set all 4 vector values to 1.0
}