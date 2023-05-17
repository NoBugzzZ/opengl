#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

float near=.1f;
float far=100.f;

float LinearizeDepth(float depth)
{
    float zNDC=2.*depth-1.;
    return 2.*near*far/(-zNDC*(far-near)+near+far);
}

void main(){
    FragColor=vec4(lightColor,1.f);

    // //线性深度
    // // float depth=LinearizeDepth(gl_FragCoord.z)/(far-near);
    // FragColor=vec4(vec3(gl_FragCoord.z),1.f);
}