#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

void main(){
    FragColor=vec4(0.0f,1.0f,0.0f,1.0f);
}