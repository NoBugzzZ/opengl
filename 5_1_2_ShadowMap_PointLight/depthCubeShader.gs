#version 330 core
layout(triangles)in;
layout(triangle_strip,max_vertices=18)out;

uniform mat4 pointLightViews[6];
uniform mat4 perspective;

out vec4 FragPos;
// flat out int layer;
void main(){
  for(int face=0;face<6;face++){
    // layer=face;
    for(int i=0;i<3;i++){
      FragPos=gl_in[i].gl_Position;
      gl_Position=perspective*pointLightViews[face]*FragPos;
      gl_Layer=face;
      EmitVertex();
    }
    EndPrimitive();
  }
}