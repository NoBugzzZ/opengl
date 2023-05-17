#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec3 diffuseMapColor=vec3(0.f);
    diffuseMapColor+=vec3(texture(texture_diffuse1,TexCoord).rgb);

    FragColor=vec4(diffuseMapColor,1.f);
}
