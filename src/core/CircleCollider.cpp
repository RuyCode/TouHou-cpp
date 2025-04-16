#include "core/CircleCollider.h"

CircleCollider::CircleCollider(float radius, glm::vec2 offset) : radius(radius), Collider(offset) {};

float CircleCollider::GetRadius() const {
    return radius;
}

float CircleCollider::GetMaxCollisionDistance() {
    return radius;
}