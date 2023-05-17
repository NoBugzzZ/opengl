#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

class Shader
{
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char *vertexPath, const char *fragmentPath);
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath);
    void setInt(const char * name,int value);
    void setFloat(const char * name,float value);
    void setBool(const char * name,bool value);
    void setVector3f(const char * name, int count,const float* value);
    void setVector2f(const char * name, int count,const float* value);
    void setMatrix4f(const char * name, int count,const float* value);
};
#endif