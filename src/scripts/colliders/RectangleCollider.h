#include "scripts/colliders/Collider.h"

#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include <glm/glm.hpp>

class RectangleCollider : public Collider {
private:
    float angle;

    float width;
    float height;

    glm::vec2 vertices[4];

    void calculateVertices();

public:
    RectangleCollider(float width, float height, float angle, glm::vec2 offset);

    void Rotate(float rotationAngle);

    float GetWidth() const;

    float GetHeight() const;

    const glm::vec2& operator[](std::size_t index) const;

    float GetMaxCollisionDistance() override;
};

#endif // RECTANGLE_COLLIDER_H