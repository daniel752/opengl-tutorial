#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 position, front, up;
    float yaw, pitch, fov;

public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov);
    ~Camera();

    glm::mat4 lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    void move(glm::vec3 direction);

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
};
