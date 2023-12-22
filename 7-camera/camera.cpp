#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov)
{
    this->position = position;
    this->front = front;
    this->up = up;
    yaw = -90.f;
    pitch = 0.0f;
    this->fov = fov;
}

Camera::~Camera()
{

}

glm::mat4 Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    return glm::lookAt(position, position + front, up);
}

void Camera::move(glm::vec3 direction)
{
    position += direction;
}