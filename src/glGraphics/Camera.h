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

    inline void SetPositionX(float x);

    inline void SetPositionY(float y);

    inline void SetPositionZ(float z);

    glm::vec3 GetPosition() const;

    glm::vec3 GetDirection() const;

    void LookAt(glm::vec3 target);

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetPerspectiveMatrix() const;

    glm::mat4 GetViewPerspectiveMatrix() const;
};

#endif // CAMERA_H
