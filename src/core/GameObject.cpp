#include "core/GameObject.h"

GameObject::GameObject(glm::vec2 position, std::shared_ptr<Collider> collider)
  : position(position),
    collider(collider),
    uuid(boost::uuids::random_generator()()),
    isActive(true) {
}

GameObject::GameObject(const GameObject& other)
  : position(other.position),
    collider(other.collider),
    uuid(other.uuid),
    isActive(other.isActive) {}

bool GameObject::operator==(const GameObject& other) const {
    return uuid == other.uuid;
}

std::shared_ptr<Collider> GameObject::GetCollider() {
  return collider;
}

glm::vec2 GameObject::GetPosition() {
  return position;
}