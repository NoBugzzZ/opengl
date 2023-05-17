#version 330 core

out vec4 FragColor;

void main(){
    if(gl_FragCoord.x<600){
        FragColor=vec4(.5f,.5f,.2f,1.f);
    }else if(gl_FragCoord.x>600){
        FragColor=vec4(0.f,.7f,.7f,1.f);
    }
}