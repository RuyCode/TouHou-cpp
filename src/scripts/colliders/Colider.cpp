#include "scripts/colliders/Collider.h"

Collider::Collider(glm::vec2 offset) : offset(offset) {}

glm::vec2 Collider::GetOffset() const {
    return offset;
}