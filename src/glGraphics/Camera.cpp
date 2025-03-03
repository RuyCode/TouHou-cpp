#include "glGraphics/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera() {
    position = glm::vec3(0.f, 0.f, 0.f);
    direction = glm::vec3(0.f, 0.f, -1.f);

    FOV = glm::radians(45.f);
    Aspect = 16.f / 9.f;
    Near = .1f;
    Far = 100.f;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction) {
    this->position = position;
    this->direction = direction;

    FOV = glm::radians(45.f);
    Aspect = 16.f / 9.f;
    Near = .1f;
    Far = 100.f;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up,  float fov, float aspect, float near, float far) {
    this->position = position;
    this->direction = direction;
    this->up = up;

    FOV = fov;
    Aspect = aspect;
    Near = near;
    Far = far;
}

void Camera::SetPosition(glm::vec3 position) {
    this->position = position;
}

void Camera::SetPosition(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Camera::SetDirection(glm::vec3 direction) {
    this->direction = direction;
}

void Camera::SetPositionX(float x) {
    this->position.x = x;
}

void Camera::SetPositionY(float y) {
    this->position.y = y;
}

void Camera::SetPositionZ(float z) {
    this->position.z = z;
}

glm::vec3 Camera::GetPosition() {
    return position;
}

glm::vec3 Camera::GetDirection() {
    return direction;
}

void Camera::LookAt(glm::vec3 target) {
    direction = glm::normalize(target - position);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::GetPerspectiveMatrix() {
    return glm::perspective(FOV, Aspect, Near, Far);
}

glm::mat4 Camera::GetViewPerspectiveMatrix() {
    return GetPerspectiveMatrix() * GetViewMatrix();
}
