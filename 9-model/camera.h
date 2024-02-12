#pragma once

#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Enum for camera movement direction
*/
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

/**
 * @brief Class Camera to handle all camera operations and logic.
*/
class Camera
{
private:
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
    /**
     * @brief Constructor to initialise camera with vectors.
     * @param position Vector3 (x,y,z) to define camera's starting position in world space.
     * @param up Vector3 (x,y,z) to define camera's up direction.
     * @param yaw Starting YAW (rotation around Y axis) value, should be -90.0f.
     * @param pitch Starting PITCH (rotation around X axis) value, should be 0.0f.
    */
    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH);

    /**
     * @brief Constructor to initialise camera with scalar values.
     * @param xPos Scalar value on X-axis for position vector.
     * @param yPos Scalar value on Y-axis for position vector.
     * @param zPos Scalar value on Z-axis for position vector.
     * @param xUp Scalar value on X-axis for up vector.
     * @param yUp Scalar value on Y-axis for up vector.
     * @param zUp Scalar value on Z-axis for up vector.
     * @param yaw Starting YAW (rotation around Y axis) value, should be -90.0f.
     * @param pitch Starting PITCH (rotation around X axis) value, should be 0.0f.
    */
    Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw = YAW, float pitch = PITCH);
    ~Camera();

    // Get view matrix using GLM "lookAt" function
    // glm::mat4 getViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    /**
     * @brief Calculates camera's translation (movement) and rotation so it will look towards target.
     * @param position Position of camera.
     * @param target Target's position.
     * @param up Camera's up direction.
    */
    glm::mat4 calculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    /**
     * @brief Process keyboard inputs to ouput movement.
     * @param direction Direction which camera should move.
     * @param deltaTime Time between frames.
    */
    void processKeyboard(CameraMovement direction, float deltaTime);

    /**
     * @brief Process mouse movement to move camera's view.
     * @param xOffset Offset on X-axis.
     * @param yOffset Offset on Y-axis.
     * @param constrainPitch Whether camera's pitch is contraint between values or not.
    */
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    /**
     * @brief Process mouse scroll to update camera's FOV.
     * @param yOffset Offset on Y-axis.
    */
    void processMouseScroll(float yOffset);

    void setPosition(glm::vec3 position) { this->position = position; };
    glm::vec3 getPosition() const { return position; };

    void setFront(glm::vec3 front) { this->front = front; };
    glm::vec3 getFront() const { return front; };

    void setUp(glm::vec3 up) { this->up = up; };
    glm::vec3 getUp() const { return up; };

    void setYaw(float yaw) { this->yaw = yaw; };
    float getYaw() { return yaw; };

    void setPitch(float pitch) { this->pitch = pitch; };
    float getPitch() { return pitch; };

    void setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; };
    float getMovementSpeed() const { return movementSpeed; };

    void setMouseSensitivity(float mouseSensitivity) { this->mouseSensitivity = mouseSensitivity; };
    float getMouseSensitivity() const { return mouseSensitivity; };

    void setFov(float fov) { this->fov = fov; };
    float getFov() { return fov; };

private:
    /**
     * @brief Calculate camera's front direction with updated Euler angles.
    */
    void updateCameraVectors();
};
