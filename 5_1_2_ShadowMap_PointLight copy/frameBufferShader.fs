#version 330 core

in vec2 TexCoord;

uniform sampler2D depthMap;

out vec4 FragColor;

void main()
{
    float depth=texture(depthMap,TexCoord).r;
    FragColor=vec4(vec3(depth),1.f);
    // FragColor=vec4(0.f,1.0f,1.0f,1.f);
}