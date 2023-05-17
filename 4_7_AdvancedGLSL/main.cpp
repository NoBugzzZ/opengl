#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <functional>
#include <sstream>
#include <map>
#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Cube.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Point.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xPos, double yPos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

unsigned int loadTexture(const char *path);

unsigned int loadCubeTexture(std::vector<std::string> paths);

int g_width = 3840;
int g_height = 2160;

float deltaTime{};
float lastTime{};

float lastX{};
float lastY{};

bool firstMouse{true};

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool isLineMode = false;
bool keyLock = false;

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

    Shader redCubeShader{"./UniformShader.vs", "./UniformShader.fs"};
    Cube redCube{};
    Shader greenCubeShader{"./UniformShader.vs", "./UniformShader.fs"};
    Cube greenCube{};
    Shader blueCubeShader{"./UniformShader.vs", "./UniformShader.fs"};
    Cube blueCube{};
    Shader yellowCubeShader{"./UniformShader.vs", "./UniformShader.fs"};
    Cube yellowCube{};

    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(redCubeShader.ID, "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(greenCubeShader.ID, "Matrices");
    unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(blueCubeShader.ID, "Matrices");
    unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(yellowCubeShader.ID, "Matrices");

    glUniformBlockBinding(redCubeShader.ID, uniformBlockIndexRed, 0);
    glUniformBlockBinding(greenCubeShader.ID, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(blueCubeShader.ID, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(yellowCubeShader.ID, uniformBlockIndexYellow, 0);

    unsigned int UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(window, scroll_callback);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4{1.0f};
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        glm::mat4 perspective = glm::mat4{1.0f};
        perspective = glm::perspective(glm::radians(45.0f), (float)g_width / (float)g_height, 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(perspective));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3(-1.f, -1.f, 0.f));
        glUseProgram(redCubeShader.ID);
        redCubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        redCubeShader.setVector3f("Color", 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
        redCube.Draw(isLineMode);

        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3(-1.f, 1.f, 0.f));
        glUseProgram(greenCubeShader.ID);
        greenCubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        greenCubeShader.setVector3f("Color", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
        greenCube.Draw(isLineMode);

        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3(1.f, -1.f, 0.f));
        glUseProgram(greenCubeShader.ID);
        greenCubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        greenCubeShader.setVector3f("Color", 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
        greenCube.Draw(isLineMode);

        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3(1.f, 1.f, 0.f));
        glUseProgram(yellowCubeShader.ID);
        yellowCubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        yellowCubeShader.setVector3f("Color", 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
        yellowCube.Draw(isLineMode);

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
    if (!keyLock && (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
    {
        isLineMode = !isLineMode;
        keyLock = true;
    }
    if (keyLock && (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE))
    {
        keyLock = false;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.scroll(xoffset, yoffset);
}

unsigned int loadTexture(const char *path)
{
    // stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);

    std::cout << "load image" << std::endl;
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Success to load texture" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

unsigned int loadCubeTexture(std::vector<std::string> paths)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (int i = 0; std::string path : paths)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        std::cout << "load image " << path << std::endl;
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            std::cout << "Success to load image " << path << std::endl;
        }
        else
        {
            std::cerr << "Failed to load image " << path << std::endl;
        }
        stbi_image_free(data);
        i++;
    }

    return texture;
}