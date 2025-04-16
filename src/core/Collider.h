#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>

class Collider {
private:
    glm::vec2 offset;

public:
    Collider(glm::vec2 offset);

    virtual float GetMaxCollisionDistance() = 0;
};

#endif // COLLIDER_H