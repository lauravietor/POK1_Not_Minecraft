#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 TexCoords;

uniform sampler2DArray myTexture;
uniform mat4 model;
uniform vec3 LightDirection;
uniform float ambient;

void main()
{
    vec3 BaseColor = texture(myTexture, TexCoords).xyz;
    float diffuse = max(dot(Normal, mat3(model) * (-LightDirection)), 0.0);
    FragColor = vec4((diffuse + ambient) * BaseColor, 1.0);
}
