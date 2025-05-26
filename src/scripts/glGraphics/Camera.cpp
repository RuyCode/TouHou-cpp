#include "scripts/glGraphics/Camera.h"
#include "scripts/utils/Logger.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera() 
    : Camera(glm::vec3(0.f), glm::vec3(0.f, 0.f, -1.f)) {}

Camera::Camera(glm::vec3 position, glm::vec3 direction) 
    : Camera(position, direction, glm::vec3(0.f, 1.f, 0.f), glm::radians(45.f), 16.f / 9.f, 0.1f, 100.f) {}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov, float aspect, float near, float far) 
    : position(position), direction(glm::normalize(direction)), up(up), 
      FOV(fov), Aspect(aspect), Near(near), Far(far) {
    
    Logger::Instance().Info("Created camera with FOV: ", FOV, ", aspect: ", Aspect);
}

void Camera::SetPosition(glm::vec3 position) {
    this->position = position; 
}

void Camera::SetPosition(float x, float y, float z) {
    this->position = glm::vec3(x, y, z); 
}

void Camera::SetDirection(glm::vec3 direction) {
    this->direction = glm::normalize(direction);
}

inline void Camera::SetPositionX(float x) { 
    position.x = x; 
}

inline void Camera::SetPositionY(float y) { 
    position.y = y; 
}

inline void Camera::SetPositionZ(float z) {
    position.z = z; 
}

glm::vec3 Camera::GetPosition() const {
    return position; 
}

glm::vec3 Camera::GetDirection() const { 
    return direction; 
}

void Camera::LookAt(glm::vec3 target) { 
    direction = glm::normalize(target - position); 
}

glm::mat4 Camera::GetViewMatrix() const { 
    return glm::lookAt(position, position + direction, up); 
}

glm::mat4 Camera::GetPerspectiveMatrix() const {    
    return glm::perspective(FOV, Aspect, Near, Far); 
}

glm::mat4 Camera::GetViewPerspectiveMatrix() const { 
    return GetPerspectiveMatrix() * GetViewMatrix(); 
}
