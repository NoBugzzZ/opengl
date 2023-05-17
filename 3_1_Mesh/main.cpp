#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <functional>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "LightCube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xPos, double yPos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

unsigned int loadTexture(const char *path);

int g_width = 3840;
int g_height = 2160;

float deltaTime{};
float lastTime{};

float lastX{};
float lastY{};

bool firstMouse{true};

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

bool isLineMode=false;
bool keyLock=false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader{"./shader.vs", "./shader.fs"};
    // Mesh mesh{};
    Model modelIns{"../model/nanosuit.obj"};

    Shader lightShader{"./shader.vs", "./lightShader.fs"};
    LightCube lightCube{};

    glm::vec3 directionlightColor{1.0f, 1.0f, 1.0f};
    glm::vec3 directionlightDir{-0.2f, -1.0f, -0.3f};
    glm::vec3 pointLightColor{1.f, 1.f, 1.f};

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 2.2f, 2.0f),
        glm::vec3(2.3f, -1.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 2.0f, -3.0f),
    };

    glUseProgram(shader.ID);
    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(window, scroll_callback);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4{1.0f};
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        // float rotateAngle = glm::radians(20.f) + glfwGetTime();
        // model = glm::rotate(model, rotateAngle, glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix();

        glm::mat4 perspective = glm::mat4{1.0f};
        perspective = glm::perspective(glm::radians(camera.fov), (float)g_width / (float)g_height, 0.1f, 100.0f);

        glUseProgram(shader.ID);
        shader.setMatrix4f("model", 1, glm::value_ptr(model));
        shader.setMatrix4f("view", 1, glm::value_ptr(view));
        shader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        shader.setVector3f("viewerPos", 1, glm::value_ptr(camera.Pos));
        shader.setFloat("material.shininess", 256.f);
        // 直射光属性
        shader.setVector3f("directionLight.color", 1, glm::value_ptr(directionlightColor));
        shader.setVector3f("directionLight.direction", 1, glm::value_ptr(directionlightDir));
        shader.setVector3f("directionLight.ambient", 1, glm::value_ptr(directionlightColor * 0.2f));
        shader.setVector3f("directionLight.diffuse", 1, glm::value_ptr(directionlightColor * 0.5f));
        shader.setVector3f("directionLight.specular", 1, glm::value_ptr(directionlightColor * 1.0f));
        // 点光源属性
        for (int i = 0; glm::vec3 pos : pointLightPositions)
        {
            std::stringstream ss;
            ss << "pointLights[" << i << "]";
            std::string str = ss.str();
            shader.setVector3f((str + ".color").c_str(), 1, glm::value_ptr(pointLightColor));
            shader.setVector3f((str + ".position").c_str(), 1, glm::value_ptr(pos));
            shader.setVector3f((str + ".ambient").c_str(), 1, glm::value_ptr(pointLightColor * 0.2f));
            shader.setVector3f((str + ".diffuse").c_str(), 1, glm::value_ptr(pointLightColor * 0.5f));
            shader.setVector3f((str + ".specular").c_str(), 1, glm::value_ptr(pointLightColor * 1.0f));
            shader.setFloat((str + ".constant").c_str(), 1.f);
            shader.setFloat((str + ".linear").c_str(), 0.022f);
            shader.setFloat((str + ".quadratic").c_str(), 0.0019f);
            i++;
        }
        // mesh.Draw(shader);
        modelIns.Draw(shader,isLineMode);

        glUseProgram(lightShader.ID);
        lightShader.setMatrix4f("view", 1, glm::value_ptr(view));
        lightShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        lightShader.setVector3f("lightColor", 1, glm::value_ptr(pointLightColor));
        for (int i = 0; glm::vec3 pos : pointLightPositions)
        {
            // pointLightColor.x = sin(glfwGetTime() * (1.f + 2.0f * i)) + 0.5f;
            // pointLightColor.y = sin(glfwGetTime() * (1.f + 0.7f * i)) + 0.5f;
            // pointLightColor.z = sin(glfwGetTime() * (1.f + 1.3f * i)) + 0.5f;
            glm::mat4 lightModel = glm::mat4{1.0f};
            lightModel = glm::translate(lightModel, pos);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));
            lightShader.setMatrix4f("model", 1, glm::value_ptr(lightModel));
            lightCube.Draw();
            i++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xPos = static_cast<float>(xposIn);
    float yPos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    // std::cout << xPos << " " << lastX << " | " << lastY << " " << yPos << " | " << xOffset << " " << yOffset << std::endl;
    lastX = xPos;
    lastY = yPos;

    camera.rotate(xOffset, yOffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // glfwSetWindowShouldClose(window, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    float current = glfwGetTime();
    deltaTime = current - lastTime;
    lastTime = current;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(Direction::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(Direction::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(Direction::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(Direction::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.move(Direction::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.move(Direction::DOWN, deltaTime);
    }
    if (!keyLock&&(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
    {   
        isLineMode=!isLineMode;
        keyLock=true;
    }
    if (keyLock&&(glfwGetKey(window,GLFW_KEY_L)==GLFW_RELEASE)){
        keyLock=false;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.scroll(xoffset, yoffset);
}
