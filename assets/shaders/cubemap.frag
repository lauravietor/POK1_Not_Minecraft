#version 330 core
out vec4 FragColor;

in vec3 vPos;
uniform samplerCube cubemap;

void main(void)
{
    FragColor = texture(cubemap, vPos);
}
