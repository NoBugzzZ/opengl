// #version 330 core
// layout(triangles)in;
// layout(line_strip,max_vertices=6)out;

// in VS_OUT{
//     vec3 Normal;
//     vec3 FragPos;
//     vec2 TexCoord;
// }gs_in[];

// void GenerateLine(int index){
//     gl_Position=gl_in[index].gl_Position;
//     EmitVertex();
//     gl_Position=gl_in[index].gl_Position+0.03f*vec4(gs_in[index].Normal,0.0f);
//     EmitVertex();
//     EndPrimitive();
// }

// void main(){
//     GenerateLine(0);
//     GenerateLine(0);
//     GenerateLine(0);
// }

#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.03;

uniform mat4 perspective;

void GenerateLine(int index)
{
    gl_Position = perspective * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = perspective * (gl_in[index].gl_Position + 
                                vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}