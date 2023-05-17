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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    Shader shader{"./shader.vs", "./shader.fs"};

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader lightShader{"./shader.vs", "./lightShader.fs"};
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glUseProgram(shader.ID);
    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(window, scroll_callback);

    glm::vec3 objectColor{1.0f, 0.5f, 0.31f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightDir{-0.2f, -1.0f, -0.3f};
    glm::vec3 pointLightColor{1.0f, 0.5f, 0.5f};
    glm::vec3 spotLightColor{1.0f, 1.0f, 1.0f};

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};

    unsigned int texture_diffuse = loadTexture("../images/container2.png");
    unsigned int texture_specular = loadTexture("../images/container2_specular.png");
    // unsigned int texture_specular = loadTexture("../images/lighting_maps_specular_color.png");
    glUniform1i(glGetUniformLocation(shader.ID, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "specularMap"), 1);

    unsigned int texture_spotmap = loadTexture("../images/awesomeface.png");
    glUniform1i(glGetUniformLocation(shader.ID, "spotLightMap"), 2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4{1.0f};
        // float rotateAngle = glm::radians(20.f) + glfwGetTime();
        // model = glm::rotate(model, rotateAngle, glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix();

        glm::mat4 perspective = glm::mat4{1.0f};
        perspective = glm::perspective(glm::radians(camera.fov), (float)g_width / (float)g_height, 0.1f, 100.0f);

        glUseProgram(shader.ID);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform3fv(glGetUniformLocation(shader.ID, "viewerPos"), 1, glm::value_ptr(camera.Pos));
        glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 256.0f);

        glm::vec3 ambient = lightColor * 0.2f;
        glm::vec3 diffuse = lightColor * 0.5f;
        glm::vec3 specular = lightColor * 1.f;

        glUniform3fv(glGetUniformLocation(shader.ID, "directionLight.direction"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(shader.ID, "directionLight.ambient"), 1, glm::value_ptr(ambient));
        glUniform3fv(glGetUniformLocation(shader.ID, "directionLight.diffuse"), 1, glm::value_ptr(diffuse));
        glUniform3fv(glGetUniformLocation(shader.ID, "directionLight.specular"), 1, glm::value_ptr(specular));

        for (int i = 0; glm::vec3 pos : pointLightPositions)
        {
            std::stringstream ss;
            ss << "pointLights[" << i << "]";
            std::string str = ss.str();
            glUniform3fv(glGetUniformLocation(shader.ID, (str + ".position").c_str()), 1, glm::value_ptr(pos));
            glUniform3fv(glGetUniformLocation(shader.ID, (str + ".ambient").c_str()), 1, glm::value_ptr(ambient));
            glUniform3fv(glGetUniformLocation(shader.ID, (str + ".diffuse").c_str()), 1, glm::value_ptr(diffuse));
            glUniform3fv(glGetUniformLocation(shader.ID, (str + ".specular").c_str()), 1, glm::value_ptr(specular));
            glUniform1f(glGetUniformLocation(shader.ID, (str + ".constant").c_str()), 1.f);
            glUniform1f(glGetUniformLocation(shader.ID, (str + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(shader.ID, (str + ".quadratic").c_str()), 0.032f);
            i++;
        }

        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.position"), 1, glm::value_ptr(camera.Pos));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.direction"), 1, glm::value_ptr(camera.Front));

        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.cutoff"), cos(glm::radians(10.f)));
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.outCutoff"), cos(glm::radians(15.f)));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.ambient"), 1, glm::value_ptr(ambient));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.diffuse"), 1, glm::value_ptr(diffuse));
        glUniform3fv(glGetUniformLocation(shader.ID, "spotLight.specular"), 1, glm::value_ptr(specular));
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.constant"), 1.f);
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.linear"), 0.045f);
        glUniform1f(glGetUniformLocation(shader.ID, "spotLight.quadratic"), 0.0075f);
        glUniform1f(glGetUniformLocation(shader.ID, "screen.width"), (float)g_width);
        glUniform1f(glGetUniformLocation(shader.ID, "screen.height"), (float)g_height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_specular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture_spotmap);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model2, cubePositions[i]);
            float angle = 20.0f * i;
            model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model2));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        // lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        glUseProgram(lightShader.ID);
        glBindVertexArray(lightVAO);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));

        for (int i = 0; glm::vec3 pos : pointLightPositions)
        {
            // pointLightColor.x = sin(glfwGetTime() * (1.f + 2.0f * i)) + 0.5f;
            // pointLightColor.y = sin(glfwGetTime() * (1.f + 0.7f * i)) + 0.5f;
            // pointLightColor.z = sin(glfwGetTime() * (1.f + 1.3f * i)) + 0.5f;
            glm::mat4 lightModel = glm::mat4{1.0f};
            lightModel = glm::translate(lightModel, pos);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
            glUniform3fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor));
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.scroll(xoffset, yoffset);
}

unsigned int loadTexture(const char *path)
{
    stbi_set_flip_vertically_on_load(true);
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