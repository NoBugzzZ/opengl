#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewerPos;
//sampler2D 不能放在struct中!!!!!!!
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;
uniform float yOffset;

struct Material{
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
    vec3 diffuseMapColor=vec3(texture(diffuseMap,TexCoord).rgb);
    vec3 specularMapColor=vec3(texture(specularMap,TexCoord).rgb);
    // vec3 specularMapColor=vec3(1.f)-vec3(texture(specularMap,TexCoord).rgb);
    vec3 ambient=light.ambient*diffuseMapColor;
    
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(lightDir,norm),0.f);
    vec3 diffuse=light.diffuse*diff*diffuseMapColor;
    
    vec3 viewerDir=normalize(viewerPos-FragPos);
    vec3 halfVec=normalize(viewerDir+lightDir);
    float spec=pow(max(dot(norm,halfVec),0.f),material.shininess);
    vec3 specular=light.specular*spec*specularMapColor;
    
    vec3 emission=vec3(texture(emissionMap,vec2(TexCoord.x,TexCoord.y+yOffset)));
    vec3 result=ambient+diffuse+specular+emission;
    FragColor=vec4(result,1.f);
}