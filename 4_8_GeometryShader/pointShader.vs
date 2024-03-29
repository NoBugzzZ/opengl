#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

out VS_OUT{
    vec3 color;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main(){
    gl_Position=perspective*view*model*vec4(aPos,1.0f);
    // gl_PointSize=100;
    vs_out.color=aColor;
}