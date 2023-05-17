#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;

out VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position=perspective*view*model*vec4(aPos,1.f);
    vs_out.Normal=normalize(mat3(transpose(inverse(perspective*view*model)))*aNormal);
    vs_out.FragPos=vec3(model*vec4(aPos,1.f));
    vs_out.TexCoord=aTexCoord;
}