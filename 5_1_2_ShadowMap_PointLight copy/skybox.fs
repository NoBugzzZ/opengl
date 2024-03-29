#version 330 core
in vec3 TexCoord;

uniform samplerCube skybox;

out vec4 FragColor;

void main(){
  FragColor=vec4(vec3(texture(skybox,TexCoord).x),1.0f);
}