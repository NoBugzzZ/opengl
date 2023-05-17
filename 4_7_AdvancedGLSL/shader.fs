#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_diffuse2;
// uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
// uniform sampler2D texture_specular2;
// uniform sampler2D texture_specular3;
uniform sampler2D texture_reflect1;
uniform samplerCube skybox;

uniform vec3 viewerPos;
//sampler2D 不能放在struct中!!!!!!!
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform sampler2D spotLightMap;

struct Material{
    float shininess;
};
uniform Material material;

struct DirectionLight{
    vec3 color;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight directionLight;

struct PointLight{
    vec3 color;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
#define NUMBER_POINT_LIGHTS 4
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outCutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    vec3 color;
};
uniform SpotLight spotLight;

struct Screen{
    float width;
    float height;
};
uniform Screen screen;

vec3 calculateDirectionLight(DirectionLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor);
vec3 calculatePointLight(PointLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor);
vec3 calculateSpotLight(SpotLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor);

float near=.1f;
float far=100.f;

float LinearizeDepth(float depth)
{
    float zNDC=2.*depth-1.;
    return 2.*near*far/(-zNDC*(far-near)+near+far);
}

void main()
{
    vec3 result=vec3(0.f);
    
    vec3 normal=normalize(Normal);
    vec3 viewDir=normalize(viewerPos-FragPos);
    vec3 diffuseMapColor=vec3(0.0f);
    vec3 specularMapColor=vec3(0.0f);
    diffuseMapColor=vec3(texture(texture_diffuse1,TexCoord).rgb);
    specularMapColor=vec3(texture(texture_specular1,TexCoord).rgb);
    
    vec3 reflectRatio=vec3(texture(texture_reflect1,TexCoord).rgb);
    vec3 reflectDir=reflect(-viewDir,normal);
    vec3 reflectColor=reflectRatio*vec3(texture(skybox,reflectDir));
    
    // result+=calculateDirectionLight(directionLight,normal,viewDir,diffuseMapColor,specularMapColor);
    // result+=calculateSpotLight(spotLight,normal,viewDir,diffuseMapColor,specularMapColor);
    // for(int i=0;i<NUMBER_POINT_LIGHTS;i++){
        //         result+=calculatePointLight(pointLights[i],normal,viewDir,diffuseMapColor,specularMapColor);
    // }
    diffuseMapColor=(vec3(1.0f)-reflectRatio)*diffuseMapColor;
    FragColor=vec4(reflectColor,1.0f);
    // FragColor=vec4(diffuseMapColor.rgb,1.);
    
    // //线性深度
    // // float depth=LinearizeDepth(gl_FragCoord.z)/(far-near);
    // FragColor=vec4(vec3(gl_FragCoord.z),1.f);
}

vec3 calculateDirectionLight(DirectionLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor){
    vec3 ambient=light.ambient*diffuseMapColor;
    
    vec3 lightDir=normalize(-light.direction);
    float diff=max(dot(lightDir,normal),0.f);
    vec3 diffuse=light.diffuse*diff*diffuseMapColor;
    
    vec3 halfVec=normalize(viewDir+lightDir);
    float spec=pow(max(dot(normal,halfVec),0.f),material.shininess);
    vec3 specular=light.specular*spec*specularMapColor;
    
    return ambient+diffuse+specular;
}

vec3 calculatePointLight(PointLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor){
    
    float d=length(light.position-FragPos);
    float attenuation=1.f/(light.constant+light.linear*d+light.quadratic*d*d);
    
    vec3 ambient=light.ambient*diffuseMapColor;
    ambient*=attenuation;
    
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(lightDir,normal),0.f);
    vec3 diffuse=light.diffuse*diff*diffuseMapColor;
    diffuse*=attenuation;
    
    vec3 halfVec=normalize(viewDir+lightDir);
    float spec=pow(max(dot(normal,halfVec),0.f),material.shininess);
    vec3 specular=light.specular*spec*specularMapColor;
    specular*=attenuation;
    
    return ambient+diffuse+specular;
}

vec3 calculateSpotLight(SpotLight light,vec3 normal,vec3 viewDir,vec3 diffuseMapColor,vec3 specularMapColor){
    vec3 result=vec3(0.f);
    
    float d=length(light.position-FragPos);
    float attenuation=1.f/(light.constant+light.linear*d+light.quadratic*d*d);
    
    vec3 lightDir=normalize(light.position-FragPos);
    float theta=dot(-lightDir,normalize(light.direction));
    
    float intensity=clamp((theta-light.outCutoff)/(light.cutoff-light.outCutoff),0.f,1.f);
    
    vec3 ambient=light.ambient*diffuseMapColor;
    ambient*=attenuation*intensity;
    result+=ambient;
    if(theta>light.outCutoff){
        float diff=max(dot(lightDir,normal),0.f);
        
        vec2 screenCoord=vec2(gl_FragCoord.x/screen.width,gl_FragCoord.y/screen.height);
        vec2 zoomCoord=vec2((screenCoord.x+.5f)/2,(screenCoord.y+.5f)/2);
        vec3 spotLightColor=vec3(texture(spotLightMap,screenCoord).rgb);
        vec3 diffuse=diff*diffuseMapColor*spotLightColor*3.f;
        // vec3 diffuse=light.diffuse*diff*diffuseMapColor*spotLightColor;
        diffuse*=attenuation*intensity;
        result+=diffuse;
        
        vec3 halfVec=normalize(viewDir+lightDir);
        float spec=pow(max(dot(normal,halfVec),0.f),material.shininess);
        vec3 specular=light.specular*spec*specularMapColor;
        specular*=attenuation*intensity;
        result+=specular;
    }
    
    return result;
}