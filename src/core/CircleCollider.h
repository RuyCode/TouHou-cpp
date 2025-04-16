#include "core/Collider.h"

#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

class CircleCollider : public Collider {
private:
    float radius;

public:
    CircleCollider(float radius, glm::vec2 offset);

    float GetRadius() const;

    float GetMaxCollisionDistance() override;
};

#endif // CIRCLE_COLLIDER_H