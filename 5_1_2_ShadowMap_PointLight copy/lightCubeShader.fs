#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightColor;

out vec4 FragColor;

void main(){
  FragColor=vec4(lightColor,1.0f);
}