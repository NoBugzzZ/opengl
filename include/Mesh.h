#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include "glm/glm.hpp"
#include <assimp/scene.h>

#include "Shader.h"

struct Vertex
{
    glm::vec3 Position{};
    glm::vec3 Normal{};
    glm::vec2 TexCoords{};
};

struct Texture
{
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh
{
private:
    void setupMesh();

public:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();

    void Draw(Shader shader, bool isLineMode = false);
};

Mesh::Mesh()
{
    std::cout << "default" << std::endl;
    this->vertices = {};
    this->textures = {};
    this->indices = {0, 1, 2};

    Vertex vertex1{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)};
    Vertex vertex2{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)};
    Vertex vertex3{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)};
    this->vertices.push_back(vertex1);
    this->vertices.push_back(vertex2);
    this->vertices.push_back(vertex3);

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->textures = textures;
    this->indices = indices;

    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::Draw(Shader shader, bool isLineMode)
{
    unsigned int diffNum = 1;
    unsigned int specNum = 1;
    unsigned int reflNum = 1;
    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        if (textures[i].type == "texture_diffuse")
        {
            number = std::to_string(diffNum++);
        }
        else if (textures[i].type == "texture_specular")
        {
            number = std::to_string(specNum++);
        }
        else if (textures[i].type == "texture_reflect")
        {
            number = std::to_string(reflNum++);
        }
        shader.setInt((textures[i].type + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    if (isLineMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

#endif