#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Plane.h"
#include "FrameBuffer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xPos, double yPos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

unsigned int loadTexture(const char *path);

unsigned int loadCubeTexture(std::vector<std::string> paths);

int g_width = 3840;
int g_height = 2160;

int shadowMap_width = 1000;
int shadowMap_height = 1000;

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
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(window, scroll_callback);

    Shader cubeShader{"./shader.vs", "./cubeShader.fs"};
    Cube cube{};

    Plane plane{};

    Shader lightShader{"./shader.vs", "./lightCubeShader.fs"};

    unsigned int texture = loadTexture("../images/wood.png");

    glUseProgram(cubeShader.ID);
    cubeShader.setInt("woodTex", 0);

    glm::vec3 pointLightColor{1.0f, 1.0f, 1.0f};
    glm::vec3 pointLightPos{3.0f, 5.0f, -3.0f};

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -6.0f),
        glm::vec3(6.0f, 0.0f, 0.0f),
        glm::vec3(6.0f, 0.0f, -6.0f),
        glm::vec3(1.5f, 5.0f, 0.0f),
        glm::vec3(0.0f, 2.0f, -2.0f),
        glm::vec3(3.0f, 2.0f, -6.0f),
    };

    Shader depthCubeShader{"./depthCubeShader.vs", "./depthCubeShader.fs", "./depthCubeShader.gs"};

    unsigned int cubeFBO;
    glGenFramebuffers(1, &cubeFBO);

    unsigned int depthCubeMap;
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMap_width, shadowMap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader skyboxShader{"./skybox.vs", "./skybox.fs"};

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glViewport(0, 0, shadowMap_width, shadowMap_height);
        glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 pointLightModel = glm::mat4{1.0f};
        glm::mat4 pointLightPerspective = glm::mat4{1.0f};
        pointLightPerspective = glm::perspective(glm::radians(90.f), (float)shadowMap_width / (float)shadowMap_height, 1.0f, 25.f);
        std::vector<glm::mat4> pointLightViews{};
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.f, 0.f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.f, 0.f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 1.f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, -1.f, 0.0f), glm::vec3(0.0, 0.0, -1.0)));
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 0.f, 1.0f), glm::vec3(0.0, -1.0, 0.0)));
        pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 0.f, -1.0f), glm::vec3(0.0, -1.0, 0.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.f, 0.f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.f, 0.f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 1.f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, -1.f, 0.0f), glm::vec3(0.0, 0.0, -1.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 0.f, 1.0f), glm::vec3(0.0, -1.0, 0.0)));
        // pointLightViews.push_back(glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.f, 0.f, -1.0f), glm::vec3(0.0, -1.0, 0.0)));

        glUseProgram(depthCubeShader.ID);
        depthCubeShader.setVector3f("lightPos", 1, glm::value_ptr(pointLightPos));
        depthCubeShader.setMatrix4f("perspective", 1, glm::value_ptr(pointLightPerspective));
        depthCubeShader.setMatrix4f("pointLightViews[0]", 1, glm::value_ptr(pointLightViews[0]));
        depthCubeShader.setMatrix4f("pointLightViews[1]", 1, glm::value_ptr(pointLightViews[1]));
        depthCubeShader.setMatrix4f("pointLightViews[2]", 1, glm::value_ptr(pointLightViews[2]));
        depthCubeShader.setMatrix4f("pointLightViews[3]", 1, glm::value_ptr(pointLightViews[3]));
        depthCubeShader.setMatrix4f("pointLightViews[4]", 1, glm::value_ptr(pointLightViews[4]));
        depthCubeShader.setMatrix4f("pointLightViews[5]", 1, glm::value_ptr(pointLightViews[5]));
        for (glm::vec3 pos : cubePositions)
        {
            pointLightModel = glm::mat4{1.0f};
            pointLightModel = glm::translate(pointLightModel, pos);
            depthCubeShader.setMatrix4f("model", 1, glm::value_ptr(pointLightModel));
            cube.Draw(isLineMode);
        }
        pointLightModel = glm::mat4{1.0f};
        depthCubeShader.setMatrix4f("model", 1, glm::value_ptr(pointLightModel));
        plane.Draw(isLineMode);

        glViewport(0, 0, g_width, g_height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 model = glm::mat4{1.0f};
        // model = glm::translate(model, glm::vec3(5.f, 0.f, 0.f));
        glm::mat4 view = glm::mat4{1.0f};
        view = camera.getViewMatrix();
        glm::mat4 perspective = glm::mat4{1.0f};
        perspective = glm::perspective(glm::radians(camera.fov), (float)g_width / (float)g_height, 0.1f, 200.0f);

        glUseProgram(cubeShader.ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // cubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        cubeShader.setMatrix4f("view", 1, glm::value_ptr(view));
        cubeShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        cubeShader.setVector3f("viewPos", 1, glm::value_ptr(camera.Pos));
        cubeShader.setFloat("shininess", 128);
        cubeShader.setVector3f("pointLight.pos", 1, glm::value_ptr(pointLightPos));
        cubeShader.setVector3f("pointLight.ambient", 1, glm::value_ptr(0.2f * pointLightColor));
        cubeShader.setVector3f("pointLight.diffuse", 1, glm::value_ptr(0.6f * pointLightColor));
        cubeShader.setVector3f("pointLight.specular", 1, glm::value_ptr(pointLightColor));
        for (glm::vec3 pos : cubePositions)
        {
            model = glm::mat4{1.0f};
            model = glm::translate(model, pos);
            cubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
            cube.Draw();
        }

        model = glm::mat4{1.0f};
        cubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        plane.Draw();

        glUseProgram(lightShader.ID);
        model = glm::mat4{1.0f};
        model = glm::translate(model, pointLightPos);
        lightShader.setMatrix4f("model", 1, glm::value_ptr(model));
        lightShader.setMatrix4f("view", 1, glm::value_ptr(view));
        lightShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        lightShader.setVector3f("lightColor", 1, glm::value_ptr(pointLightColor));
        cube.Draw();

        glUseProgram(skyboxShader.ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3(-4.0f, 2.0f, 0.0f));
        lightShader.setMatrix4f("model", 1, glm::value_ptr(model));
        lightShader.setMatrix4f("view", 1, glm::value_ptr(view));
        lightShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        cube.Draw();

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