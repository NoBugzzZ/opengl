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

int g_width = 2560;
int g_height = 1440;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(window, scroll_callback);

    Shader cubeShader{"./shader.vs", "./cubeShader.fs"};
    Cube cube{};

    Shader planeShader{"./shader.vs", "./planeShader.fs"};
    Plane plane{};

    Shader lightShader{"./shader.vs", "./lightCubeShader.fs"};

    Shader screenShader{"./framebufferShader.vs", "./framebufferShader.fs"};
    FrameBuffer screen{};

    Shader depthShader{"./depthShader.vs", "./depthShader.fs"};

    unsigned int texture = loadTexture("../images/wood.png");
    glUseProgram(planeShader.ID);
    planeShader.setInt("woodTex", 0);
    planeShader.setInt("depthMap", 1);

    glUseProgram(cubeShader.ID);
    cubeShader.setInt("woodTex", 0);
    cubeShader.setInt("depthMap", 1);
    cubeShader.setInt("depthCubeMap", 2);

    glm::vec3 directLightColor{1.0f, 1.0f, 1.0f};
    glm::vec3 directLightCubePosition{-1.0f, 4.0f, -1.0f};
    glm::vec3 directLightDir = glm::vec3(0.0f) - directLightCubePosition;

    glm::vec3 pointLightColor{1.0f, 1.0f, 1.0f};
    glm::vec3 pointLightPos{3.0f, 3.0f, -3.0f};

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -6.0f),
        glm::vec3(6.0f, 0.0f, 0.0f),
        glm::vec3(6.0f, 0.0f, -6.0f),
        glm::vec3(0.0f, 2.5f, 0.0f),
    };

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMap_width, shadowMap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[]{1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(screenShader.ID);
    screenShader.setInt("depthMap", 0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glViewport(0, 0, shadowMap_width, shadowMap_height);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 lightModel = glm::mat4{1.0f};
        // model = glm::translate(model, glm::vec3(5.f, 0.f, 0.f));
        glm::mat4 lightView = glm::mat4(1.0f);
        lightView = glm::lookAt(directLightCubePosition, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f));
        // view = camera.getViewMatrix();
        glm::mat4 lightPerspective = glm::mat4{1.0f};
        lightPerspective = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        // perspective = glm::perspective(glm::radians(camera.fov), (float)g_width / (float)g_height, 0.1f, 200.0f);

        glUseProgram(depthShader.ID);
        // cubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        depthShader.setMatrix4f("view", 1, glm::value_ptr(lightView));
        depthShader.setMatrix4f("perspective", 1, glm::value_ptr(lightPerspective));
        // cube.Draw();
        for (glm::vec3 pos : cubePositions)
        {
            lightModel = glm::mat4{1.0f};
            lightModel = glm::translate(lightModel, pos);
            depthShader.setMatrix4f("model", 1, glm::value_ptr(lightModel));
            cube.Draw(isLineMode);
        }

        lightModel = glm::mat4{1.0f};
        depthShader.setMatrix4f("model", 1, glm::value_ptr(lightModel));
        plane.Draw(isLineMode);

        glViewport(0, 0, g_width, g_height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUseProgram(screenShader.ID);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // screen.Draw(isLineMode);

        glm::mat4 model = glm::mat4{1.0f};
        // model = glm::translate(model, glm::vec3(5.f, 0.f, 0.f));
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        glm::mat4 perspective = glm::mat4{1.0f};
        perspective = glm::perspective(glm::radians(camera.fov), (float)g_width / (float)g_height, 0.1f, 200.0f);

        glUseProgram(cubeShader.ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glActiveTexture(GL_TEXTURE2);
        // cubeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        cubeShader.setMatrix4f("view", 1, glm::value_ptr(view));
        cubeShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        cubeShader.setVector3f("viewPos", 1, glm::value_ptr(camera.Pos));
        cubeShader.setFloat("shininess", 128);
        cubeShader.setVector3f("directLight.dir", 1, glm::value_ptr(directLightDir));
        cubeShader.setVector3f("directLight.ambient", 1, glm::value_ptr(0.2f * directLightColor));
        cubeShader.setVector3f("directLight.diffuse", 1, glm::value_ptr(0.6f * directLightColor));
        cubeShader.setVector3f("directLight.specular", 1, glm::value_ptr(directLightColor));
        cubeShader.setMatrix4f("lightSpaceMatrix", 1, glm::value_ptr(lightPerspective * lightView));
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

        // glUseProgram(planeShader.ID);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // model = glm::mat4{1.0f};
        // planeShader.setMatrix4f("model", 1, glm::value_ptr(model));
        // planeShader.setMatrix4f("view", 1, glm::value_ptr(view));
        // planeShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        // planeShader.setVector3f("viewPos", 1, glm::value_ptr(camera.Pos));
        // planeShader.setFloat("shininess", 32);
        // planeShader.setVector3f("directLight.dir", 1, glm::value_ptr(directLightDir));
        // planeShader.setVector3f("directLight.ambient", 1, glm::value_ptr(0.2f * directLightColor));
        // planeShader.setVector3f("directLight.diffuse", 1, glm::value_ptr(0.6f * directLightColor));
        // planeShader.setVector3f("directLight.specular", 1, glm::value_ptr(directLightColor));
        // planeShader.setMatrix4f("lightSpaceMatrix", 1, glm::value_ptr(lightPerspective * lightView));
        // plane.Draw();

        glUseProgram(lightShader.ID);
        model = glm::mat4{1.0f};
        model = glm::translate(model, directLightCubePosition);
        lightShader.setMatrix4f("model", 1, glm::value_ptr(model));
        lightShader.setMatrix4f("view", 1, glm::value_ptr(view));
        lightShader.setMatrix4f("perspective", 1, glm::value_ptr(perspective));
        lightShader.setVector3f("lightColor", 1, glm::value_ptr(directLightColor));
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