#version 330 core
in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

const float offset=1./300.;

void main(){
    // FragColor=texture(screenTexture,TexCoords);
    
    // FragColor=vec4(vec3(1.0f)-texture(screenTexture,TexCoords).rgb,1.0f);
    
    // FragColor=texture(screenTexture,TexCoords);
    // // float grayScale=(FragColor.r+FragColor.g+FragColor.b)/3.0f;
    // float grayScale=.2126*FragColor.r+.7152*FragColor.g+.0722*FragColor.b;
    // FragColor=vec4(grayScale,grayScale,grayScale,1.f);
    
    vec2 offsets[9]=vec2[](
        vec2(-offset,offset),// 左上
        vec2(0.f,offset),// 正上
        vec2(offset,offset),// 右上
        vec2(-offset,0.f),// 左
        vec2(0.f,0.f),// 中
        vec2(offset,0.f),// 右
        vec2(-offset,-offset),// 左下
        vec2(0.f,-offset),// 正下
        vec2(offset,-offset)// 右下
    );
    
    //Sharpen
    // float kernel[9]=float[](
        //     -1,-1,-1,
        //     -1,9,-1,
        //     -1,-1,-1
    // );
    
    //blur
    // float kernel[9]=float[](
        //     1./16,2./16,1./16,
        //     2./16,4./16,2./16,
        //     1./16,2./16,1./16
    // );
    
    //Edge-detection
    // float kernel[9]=float[](
    //     1,1,1,
    //     1,-8,1,
    //     1,1,1
    // );

    //Vertical Edge-detection
    // float kernel[9]=float[](
    //     1,0,-1,
    //     2,0,-2,
    //     1,0,-1
    // );

    //Horizontal Edge-detection
    float kernel[9]=float[](
        -1,-2,-1,
        0,0,0,
        1,2,1
    );
    
    vec3 samples[9];
    for(int i=0;i<9;i++)
    {
        samples[i]=vec3(texture(screenTexture,offsets[i]+TexCoords));
    }
    
    vec3 convolution=vec3(0.f);
    for(int i=0;i<9;i++){
        convolution+=kernel[i]*samples[i];
    }
    FragColor=vec4(convolution,1.f);
}
