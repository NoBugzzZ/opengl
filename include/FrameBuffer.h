#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <vector>

#include <glad/glad.h>

class FrameBuffer
{
private:
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    void setupMesh();

public:
    FrameBuffer();
    FrameBuffer(std::vector<float> vertices);
    ~FrameBuffer();
    void Draw(bool isLineMode = false);
};

void FrameBuffer::Draw(bool isLineMode)
{
    if (isLineMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void FrameBuffer::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

FrameBuffer::FrameBuffer(std::vector<float> vertices)
{
    this->vertices = vertices;
    setupMesh();
}

FrameBuffer::FrameBuffer()
{
    this->vertices = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                      // positions   // texCoords
                      -1.0f, 1.0f, 0.0f, 1.0f,
                      -1.0f, -1.0f, 0.0f, 0.0f,
                      1.0f, -1.0f, 1.0f, 0.0f,

                      -1.0f, 1.0f, 0.0f, 1.0f,
                      1.0f, -1.0f, 1.0f, 0.0f,
                      1.0f, 1.0f, 1.0f, 1.0f};

    setupMesh();
}

FrameBuffer::~FrameBuffer()
{
}

#endif