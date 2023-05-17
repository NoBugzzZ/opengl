#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;

float farPlane=25.0f;

void main(){
  float dist=length(lightPos-FragPos.xyz);
  dist=dist/farPlane;
  gl_FragDepth=dist;
}