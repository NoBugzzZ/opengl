#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int g_width = 2560;
int g_height = 1440;

void drawTriangle();

const char *vertexShaderSource = "#version 330 core\n\
    layout (location = 0) in vec3 aPos;\n\
    out vec4 vertexColor;\n\
    void main()\n\
    {\n\
        gl_Position = vec4(aPos, 1.0f);\n\
        vertexColor=vec4(1.0f,0.0f,1.0f,1.0f);\n\
    }\0";

const char *fragmentShaderSource = "#version 330 core\n\
    out vec4 FragColor;\n\
    in vec4 vertexColor;\n\
    uniform vec4 ourColor;\n\
    void main()\n\
    {\n\
        FragColor=vec4(1.0f,0.0f,0.0f,1.0f);\n\
    }\0";

const char *fragmentShaderSource2 = "#version 330 core\n\
    out vec4 FragColor;\n\
    void main()\n\
    {\n\
        FragColor=vec4(1.0f,1.0f,0.0f,1.0f);\n\
    }\0";
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

    // std::cout<<vertexShaderSource<<'\n';
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char info[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "VERTEX SHADER COMPILE SUCCESS" << '\n';
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "FRAGMENT SHADER COMPILE SUCCESS" << '\n';
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
        std::cerr << "ERROR::LINK_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "LINK SUCCESS" << '\n';
    }

    glDeleteShader(fragmentShader);

    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, nullptr);
    glCompileShader(fragmentShader2);

    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, nullptr, info);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "FRAGMENT SHADER COMPILE SUCCESS" << '\n';
    }

    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, nullptr, info);
        std::cerr << "ERROR::LINK_FAILED\n"
                  << info << '\n';
    }
    else
    {
        std::cout << "LINK SUCCESS" << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader2);

    float vertices[2][9]{
        {
            -1.0f,
            -0.5f,
            0.0f,
            0.0f,
            -0.5f,
            0.0f,
            -0.5f,
            0.5f,
            0.0f,
        },
        {0.0f, -0.5f, 0.0f,
         1.0f, -0.5f, 0.0f,
         0.5f, 0.5f, 0.0f}};

    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f,   // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f   // top left
    // };
    // unsigned int indices[] = {
    //     // note that we start from 0!
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    unsigned int VAO[2], VBO[2], EBO;
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // glGenBuffers(1,&EBO);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // float green=sin(glfwGetTime())/2.0f+0.5f;
        // int vertexColorLocation=glGetUniformLocation(shaderProgram,"ourColor");
        // // glUniform4f(vertexColorLocation,0,green,0,1);
        // float color[4]{0,green,0,1};
        // glUniform4fv(vertexColorLocation,1,color);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
