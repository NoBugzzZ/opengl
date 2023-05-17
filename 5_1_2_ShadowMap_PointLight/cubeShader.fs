#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D woodTex;

uniform vec3 viewPos;


struct PointLight{
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

uniform float shininess;

out vec4 FragColor;

void main(){
    vec3 normal=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    
    vec3 diffuseColor=texture(woodTex,TexCoord).rgb;
    vec3 ambient=pointLight.ambient*diffuseColor;
    
    vec3 lightDir=normalize(pointLight.pos-FragPos);
    vec3 halfVec=normalize(lightDir+viewDir);
    
    vec3 diffuse=pointLight.diffuse*diffuseColor*max(dot(lightDir,normal),0);
    vec3 specular=pointLight.specular*diffuseColor*pow(max(dot(halfVec,normal),0),shininess);
    
    float distance=length(pointLight.pos-FragPos);
    float alpha=1.f/distance*distance/9.f+.1;
    vec3 lighting=ambient+diffuse+specular;

    FragColor=vec4(lighting,1.f);
}