#version 330 core
layout(location=0)in vec3 aPos;

out vec3 TexCoords;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords=aPos;
    vec4 pos=perspective*view*vec4(aPos,1.);
    gl_Position=vec4(pos.x,pos.y,pos.w-0.000001f,pos.w);
}
