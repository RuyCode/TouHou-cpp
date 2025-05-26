#include "scripts/colliders/RectangleCollider.h"

#include <glm/glm.hpp>
#include <stdexcept>

RectangleCollider::RectangleCollider(float width, float height, float angle, glm::vec2 offset)
  : width(width), 
    height(height),
    angle(angle),
    Collider(offset) {

    calculateVertices();
}

void RectangleCollider::calculateVertices() {
    float cosAngle = glm::cos(angle);
    float sinAngle = glm::sin(angle);

    float halfWidth = width * .5f;
    float halfHeight = height * .5f;

    vertices[0] = glm::vec2(halfWidth * cosAngle - halfHeight * sinAngle, halfWidth * sinAngle + halfHeight * cosAngle);
    vertices[1] = glm::vec2(-halfWidth * cosAngle - halfHeight * sinAngle, -halfWidth * sinAngle + halfHeight * cosAngle);
    vertices[2] = glm::vec2(-halfWidth * cosAngle + halfHeight * sinAngle, -halfWidth * sinAngle - halfHeight * cosAngle);
    vertices[3] = glm::vec2(halfWidth * cosAngle + halfHeight * sinAngle, halfWidth * sinAngle - halfHeight * cosAngle);
}

void RectangleCollider::Rotate(float rotationAngle) {
    float cosAngle = glm::cos(angle);
    float sinAngle = glm::sin(angle);

    
    vertices[0] = glm::vec2(vertices[0].x * cosAngle - vertices[0].y * sinAngle, vertices[0].x * sinAngle + vertices[0].y * cosAngle);
    vertices[1] = glm::vec2(-vertices[1].x * cosAngle - vertices[1].y * sinAngle, -vertices[1].x * sinAngle + vertices[1].y * cosAngle);
    vertices[2] = glm::vec2(-vertices[2].x * cosAngle + vertices[2].y * sinAngle, -vertices[2].x * sinAngle - vertices[2].y * cosAngle);
    vertices[3] = glm::vec2(vertices[3].x * cosAngle + vertices[3].y * sinAngle, vertices[3].x * sinAngle - vertices[3].y * cosAngle);
}

float RectangleCollider::GetWidth() const {
    return width;
}

float RectangleCollider::GetHeight() const {
    return height;
}

const glm::vec2& RectangleCollider::operator[](std::size_t index) const {
    if (index > 3) {
        throw std::runtime_error("Error: Rectangle collider index out of bounds");
    }

    return vertices[index];
}

float RectangleCollider::GetMaxCollisionDistance() {
    return std::max(width, height);
}
