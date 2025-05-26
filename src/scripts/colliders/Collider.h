#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>

class Collider {
protected:
    glm::vec2 offset;

public:
    Collider(glm::vec2 offset);

    glm::vec2 GetOffset() const;

    virtual float GetMaxCollisionDistance() = 0;
};

#endif // COLLIDER_H