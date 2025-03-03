#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    
public:
    float FOV;
    float Aspect;
    float Near;
    float Far;


    Camera();

    Camera(glm::vec3 position, glm::vec3 direction);

    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov, float aspect, float near, float far);

    void SetPosition(glm::vec3 position);

    void SetPosition(float x, float y, float z);

    void SetDirection(glm::vec3 direction);

    void SetPositionX(float x);

    void SetPositionY(float x);

    void SetPositionZ(float x);

    glm::vec3 GetPosition();

    glm::vec3 GetDirection();

    void LookAt(glm::vec3 target);

    glm::mat4 GetViewMatrix();

    glm::mat4 GetPerspectiveMatrix();

    glm::mat4 GetViewPerspectiveMatrix();
};

#endif // CAMERA_H