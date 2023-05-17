#version 330 core

in VS_OUT{
    vec3 testColor;
}fs_in;

out vec4 FragColor;

void main(){
    FragColor=vec4(fs_in.testColor,1.f);
}