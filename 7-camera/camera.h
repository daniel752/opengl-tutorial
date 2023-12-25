#pragma once

#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const glm::vec3 POSITION(0.0f, 0.0f, 0.0f);
const glm::vec3 UP(0.0f, 1.0f, 0.0f);
const glm::vec3 FRONT(0.0f, 0.0f, -1.0f);
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.2f;
const float FOV = 45.0f;

class Camera
{
private:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // Euler angles for camera
    float yaw;
    float pitch;
    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;

public:
    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH);
    ~Camera();

    glm::mat4 getViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    void processKeyboard(CameraMovement direction, float deltaTime);

    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    void processMouseScroll(float yOffset);

    void setPosition(glm::vec3 position) { this->position = position; };
    glm::vec3 getPosition() const { return position; };

    void setFront(glm::vec3 front) { this->front = front; };
    glm::vec3 getFront() const { return front; };

    void setUp(glm::vec3 up) { this->up = up; };
    glm::vec3 getUp() const { return up; };

    void setYaw(float yaw) { this->yaw = yaw; };
    float getYaw() { return yaw; };

    void setPitch(float pitch) {this->pitch = pitch; };
    float getPitch() { return pitch; };

    void setFov(float fov) { this->fov = fov; };
    float getFov() { return fov; };

private:
    void updateCameraVectors();
};
