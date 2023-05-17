#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D woodTex;
uniform sampler2D depthMap;
uniform samplerCube depthCubeMap;

uniform vec3 viewPos;

float far_plane=25.f;

struct DirectionLight{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight directLight;

uniform float shininess;

out vec4 FragColor;

// float calculateShadow(vec4 FragPosLightSpace,vec3 lightDir,vec3 normal){
    //     vec3 projectCoord=FragPosLightSpace.xyz/FragPosLightSpace.w;
    //     projectCoord=projectCoord*.5+.5;
    //     float closedDepth=texture(depthMap,projectCoord.xy).r;
    //     float currentDepth=projectCoord.z;
    //     currentDepth=currentDepth<1.f?currentDepth:1.f;
    //     float bias=max(0.05*(1-dot(lightDir,normal)),0.005);
    //     return currentDepth-bias>closedDepth?1.f:0.f;
// }

float calculateShadow(vec4 FragPosLightSpace,vec3 lightDir,vec3 normal){
    vec3 projectCoord=FragPosLightSpace.xyz/FragPosLightSpace.w;
    projectCoord=projectCoord*.5+.5;
    float currentDepth=projectCoord.z;
    currentDepth=currentDepth<1.f?currentDepth:1.f;
    float bias=max(.05*(1-dot(lightDir,normal)),.005);
    
    float shadow=0.f;
    vec2 texSize=1.f/textureSize(depthMap,0);
    for(int x=-1;x<=1;x++){
        for(int y=-1;y<=1;y++){
            float closedDepth=texture(depthMap,projectCoord.xy+texSize*vec2(x,y)).r;
            shadow+=currentDepth-bias>closedDepth?1.f:0.f;
        }
    }
    
    return shadow/9.f;
}

void main(){
    vec3 normal=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 lightDir=normalize(-directLight.dir);
    vec3 halfVec=normalize(lightDir+viewDir);
    
    vec3 diffuseColor=texture(woodTex,TexCoord).rgb;
    vec3 ambient=directLight.ambient*diffuseColor;
    
    vec3 diffuse=directLight.diffuse*diffuseColor*max(dot(lightDir,normal),0);
    vec3 specular=directLight.specular*diffuseColor*pow(max(dot(halfVec,normal),0),shininess);
    float shadow=calculateShadow(FragPosLightSpace,lightDir,normal);
    vec3 lighting=ambient+(diffuse+specular)*(1-shadow);
    
    FragColor=vec4(lighting,1.f);
}