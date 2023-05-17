#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

// in vec3 LightPos;

// in vec3 VertexColor;

void main()
{
    vec3 ambient=.1f*lightColor;
    vec3 norm=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    // vec3 lightDir=normalize(LightPos-FragPos);
    float diff=max(dot(lightDir,norm),0.f);
    vec3 diffuse=diff*lightColor;
    vec3 viewerDir=normalize(viewerPos-FragPos);
    // vec3 viewerDir=normalize(-FragPos);
    vec3 halfVec=normalize(viewerDir+lightDir);
    vec3 specular=pow(max(dot(norm,halfVec),0.f), 32)*lightColor;
    vec3 lightI=ambient+diffuse+specular*0.5;
    FragColor=vec4(objectColor*lightI,1.f);

    // FragColor=vec4(VertexColor,1.f);
}