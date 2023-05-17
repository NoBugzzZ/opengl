#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 ambient=light.ambient*material.ambient;
    
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(lightDir,norm),0.f);
    vec3 diffuse=light.diffuse*diff*material.diffuse;
    
    vec3 viewerDir=normalize(viewerPos-FragPos);
    vec3 halfVec=normalize(viewerDir+lightDir);
    float spec=pow(max(dot(norm,halfVec),0.f),material.shininess);
    vec3 specular=light.specular*spec*material.specular;
    
    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.f);
}