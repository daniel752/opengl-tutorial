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

Camera::Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch) :
front(FRONT),
movementSpeed(SPEED),
mouseSensitivity(SENSITIVITY),
fov(FOV)
{
    position = glm::vec3(xPos, yPos, zPos);
    worldUp = glm::vec3(xUp, yUp, zUp);
    this->yaw = yaw;
    this->pitch = pitch;
}

Camera::~Camera()
{

}

// glm::mat4 Camera::getViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up)
// {
//     return glm::lookAt(position, position + front, up);
// }

glm::mat4 Camera::calculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    // Create Z-axis by subtracting "target" vector from "position" vector
    glm::vec3 zaxis = glm::normalize(position - target);
    // Create X-axis from cross-product of world up and Z-axis
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // Create Y-axis from cross-product of Z and X axes
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // Create translation matrix for movement
    // We decrease the position because in OpenGL and generally we move the whole world around the camera instead of moving the camera itself
    glm::mat4 translation(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    // Create rotation matrix to calculate rotation in all axes
    glm::mat4 rotation(1.0f);
    rotation[0][0] = xaxis.x;
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x;
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x;
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    // Return lookAt matrix by multiplying "translation" and "rotation" matrices
    // Remember that matrix multiplication is from right to left 
    return rotation * translation;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    // Velocity equals to defined movement speed multiplied by delta time (time between frames)
    float velocity = movementSpeed * deltaTime;

    // Switch case to add velocity to specified direction
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

    // std::cout << "POSITION (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
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