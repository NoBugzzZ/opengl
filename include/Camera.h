#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Direction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera
{
private:
    void updateCamera()
    {
        glm::vec3 front{};
        front.y = sin(glm::radians(pitch));
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUP));
        Up = glm::normalize(glm::cross(Right, Front));

        glm::vec3 HFront{front.x, 0.f, front.z};
        HorizontalFront = glm::normalize(HFront);
        HorizontalRight = glm::normalize(glm::cross(HorizontalFront, WorldUP));
        // print(WorldUP);
        // print(Front);
        // print(Right);
        // print(Up);
    }
    void print(glm::vec3 vec)
    {
        std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
    }

public:
    // float yaw{-90.f};
    float yaw{0.f};
    float pitch{90.f};
    float fov{45.f};
    float speed{3.f};
    float sensitive{0.1f};
    glm::vec3 WorldUP{0.f, 1.f, 0.f};
    glm::vec3 Front{};
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 Pos{};

    glm::vec3 HorizontalFront{};
    glm::vec3 HorizontalRight{};

    Camera(glm::vec3 position = {0.f, 0.f, 3.f})
    {
        Pos = position;
        updateCamera();
        // std::cout << yaw << pitch << fov << speed << sensitive << std::endl;
    }

    void scroll(double xoffset, double yoffset)
    {
        if (fov >= 1.f && fov <= 45.f)
        {
            fov -= yoffset;
        }
        if (fov < 1.f)
        {
            fov = 1.f;
        }
        if (fov > 45.f)
        {
            fov = 45.f;
        }
    }

    void move(Direction dir, float deltaTime)
    {
        switch (dir)
        {
        case Direction::FORWARD:
            Pos += Front * speed * deltaTime;
            break;
        case Direction::BACKWARD:
            Pos -= Front * speed * deltaTime;
            break;
        case Direction::RIGHT:
            Pos += Right * speed * deltaTime;
            break;
        case Direction::LEFT:
            Pos -= Right * speed * deltaTime;
            break;
        case Direction::UP:
            Pos += Up * speed * deltaTime;
            break;
        case Direction::DOWN:
            Pos -= Up * speed * deltaTime;
            break;
        default:
            break;
        }
    }

    void rotate(double xOffset, double yOffset, bool constrainPitch = true)
    {
        yaw += xOffset * sensitive;
        pitch += yOffset * sensitive;

        if (constrainPitch)
        {
            if (pitch > 89.f)
            {
                pitch = 89.f;
            }
            if (pitch < -89.f)
            {
                pitch = -89.f;
            }
        }
        updateCamera();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Pos, Pos + Front, Up);

        // glm::mat4 moveToZero = glm::mat4(1.0f);
        // moveToZero[3][0] = -Pos.x;
        // moveToZero[3][1] = -Pos.y;
        // moveToZero[3][2] = -Pos.z;
        // glm::mat4 rotateToXYZ = glm::mat4(1.0f);
        // rotateToXYZ[0][0] = Right.x; // First column, first row
        // rotateToXYZ[1][0] = Right.y;
        // rotateToXYZ[2][0] = Right.z;
        // rotateToXYZ[0][1] = Up.x; // First column, second row
        // rotateToXYZ[1][1] = Up.y;
        // rotateToXYZ[2][1] = Up.z;
        // rotateToXYZ[0][2] = -Front.x; // First column, third row
        // rotateToXYZ[1][2] = -Front.y;
        // rotateToXYZ[2][2] = -Front.z;

        // return rotateToXYZ * moveToZero;
    }
};
#endif