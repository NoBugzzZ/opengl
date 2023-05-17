#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

layout(std140)uniform Matrices{
    mat4 perspective;
    mat4 view;
};
uniform mat4 model;

void main()
{
    gl_Position=perspective*view*model*vec4(aPos,1.f);
    // gl_Position=vec4(aPos,1.0f);
    Normal=mat3(transpose(inverse(model)))*aNormal;
    FragPos=vec3(model*vec4(aPos,1.f));
    TexCoord=aTexCoord;
}