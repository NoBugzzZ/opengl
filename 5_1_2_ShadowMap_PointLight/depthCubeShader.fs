#version 330 core
// layout(location=0)out vec4 positivex;
// layout(location=1)out vec4 negativex;
// layout(location=2)out vec4 positivey;
// layout(location=3)out vec4 negativey;
// layout(location=4)out vec4 positivez;
// layout(location=5)out vec4 negativez;

in vec4 FragPos;
// flat in int layer;

uniform vec3 lightPos;

float farPlane=25.f;

void main(){
  float dist=length(lightPos-FragPos.xyz);
  dist=dist/farPlane;
  gl_FragDepth=dist;
  // switch(layer){
  //   case 0:
  //   positivex=vec4(dist);
  //   break;
  //   case 1:
  //   negativex=vec4(dist);
  //   break;
  //   case 2:
  //   positivey=vec4(dist);
  //   break;
  //   case 3:
  //   negativey=vec4(dist);
  //   break;
  //   case 4:
  //   positivez=vec4(dist);
  //   break;
  //   case 5:
  //   negativez=vec4(dist);
  //   break;
  // }
}