#ifndef POINT_H
#define POINT_H
#include <vector>

#include <glad/glad.h>

class Point
{
private:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    void setupMesh();

public:
    Point();
    Point(std::vector<float> vertices);
    ~Point();
    void Draw(bool isLineMode = false);
};

void Point::Draw(bool isLineMode)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);
    glBindVertexArray(0);
}

void Point::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
}

Point::Point(std::vector<float> vertices)
{
    this->vertices = vertices;
    setupMesh();
}

Point::Point()
{
    this->vertices = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // top-right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f   // top-left
    };
    setupMesh();
}

Point::~Point()
{
}

#endif