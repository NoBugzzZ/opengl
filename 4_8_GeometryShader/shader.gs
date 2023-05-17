#version 330 core
layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
}gs_in[];

out GS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
}gs_out;

uniform float time;

vec3 getNormal(){
    vec3 a=vec3(gl_in[0].gl_Position-gl_in[1].gl_Position);
    vec3 b=vec3(gl_in[2].gl_Position-gl_in[1].gl_Position);
    return normalize(cross(a,b));
}

vec4 explode(vec4 position,vec3 normal){
    float magnitude=2.;
    vec3 step=normal*((sin(time)+1.)/2)*magnitude;
    return position+vec4(step,1.);
}

void main(){
    vec3 normal=getNormal();
    gl_Position=explode(gl_in[0].gl_Position,normal);
    gs_out.TexCoord=gs_in[0].TexCoord;
    EmitVertex();
    gl_Position=explode(gl_in[1].gl_Position,normal);
    gs_out.TexCoord=gs_in[1].TexCoord;
    EmitVertex();
    gl_Position=explode(gl_in[2].gl_Position,normal);
    gs_out.TexCoord=gs_in[2].TexCoord;
    EmitVertex();
    EndPrimitive();
}