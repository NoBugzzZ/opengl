#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;
layout(location=3)in mat4 instanceModelMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform bool isInstance;

void main()
{
    if(isInstance){
        gl_Position=perspective*view*instanceModelMatrix*vec4(aPos,1.f);
    }else{
        gl_Position=perspective*view*model*vec4(aPos,1.f);
    }
    
    // Normal=mat3(transpose(inverse(instanceModelMatrix)))*aNormal;
    // FragPos=vec3(instanceModelMatrix*vec4(aPos,1.f));
    TexCoord=aTexCoord;
}