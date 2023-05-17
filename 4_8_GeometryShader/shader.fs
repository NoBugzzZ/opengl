#version 330 core
out vec4 FragColor;

// in GS_OUT{
//     vec3 Normal;
//     vec3 FragPos;
//     vec2 TexCoord;
// }fs_in;

in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflect1;
uniform samplerCube skybox;

void main()
{
    vec3 diffuseMapColor=vec3(0.0f);
    diffuseMapColor=vec3(texture(texture_diffuse1,fs_in.TexCoord));
    FragColor=vec4(diffuseMapColor,1.);
}
