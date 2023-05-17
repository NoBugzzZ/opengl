#version 330 core
// in vec2 TexCoord;

// uniform vec3 color;
// uniform sampler2D texture1;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewerPos;
uniform samplerCube skybox;

out vec4 FragColor;

void main(){
    vec3 I=normalize(FragPos-viewerPos);
    vec3 ReflectDir=reflect(I,normalize(Normal));
    float ratio=1./1.52;
    vec3 RefractDir=refract(I,normalize(Normal),ratio);
    
    float alpha=.5f;
    float beta=.5f;
    vec4 texColor=alpha*texture(skybox,ReflectDir)+beta*texture(skybox,RefractDir);
    // FragColor=vec4(1,0,0,1.)*texColor;
    FragColor=texColor;
}