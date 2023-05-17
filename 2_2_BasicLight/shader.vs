#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

// out vec3 LightPos;
// uniform vec3 lightPos;

// out vec3 VertexColor;
// uniform vec3 objectColor;
// uniform vec3 lightColor;
// uniform vec3 lightPos;
// uniform vec3 viewerPos;

void main()
{
    gl_Position=perspective*view*model*vec4(aPos,1.f);
    Normal=mat3(transpose(inverse(model)))*aNormal;
    FragPos=vec3(model*vec4(aPos,1.f));

    // Normal=mat3(transpose(inverse(view*model)))*aNormal;
    // FragPos=vec3(view*model*vec4(aPos,1.f));
    // LightPos=vec3(view*vec4(lightPos,1.f));

    // Normal=mat3(transpose(inverse(model)))*aNormal;
    // FragPos=vec3(model*vec4(aPos,1.f));
    // vec3 ambient=.1f*lightColor;
    // vec3 norm=normalize(Normal);
    // vec3 lightDir=normalize(lightPos-FragPos);
    // float diff=max(dot(lightDir,norm),0.f);
    // vec3 diffuse=diff*lightColor;
    // vec3 viewerDir=normalize(viewerPos-FragPos);
    // vec3 halfVec=normalize(viewerDir+lightDir);
    // vec3 specular=pow(max(dot(norm,halfVec),0.f), 32)*lightColor;
    // vec3 lightI=ambient+diffuse+specular*0.5;
    // VertexColor=objectColor*lightI;
}