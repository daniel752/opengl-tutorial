#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
front(FRONT),
movementSpeed(SPEED),
mouseSensitivity(SENSITIVITY),
fov(FOV)
{
    this->position = position;
    worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;

    switch(direction)
    {
        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
    }

    std::cout << "POSITION (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(constrainPitch)
    {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }

    // std::cout << "PITCH " << pitch << std::endl;
    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
    fov -= yOffset;

    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > 90.0f)
        fov = 90.0f;

    // std::cout << "FOV " << fov << std::endl;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->front = glm::normalize(front);
    right = glm::cross(front, worldUp);
    up = glm::cross(right, front);

    // std::cout << "FRONT (" << front.x << ", " << front.y << ", " << front.z << ")" << std::endl;
    // std::cout << "RIGHT (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;
    // std::cout << "UP (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
}