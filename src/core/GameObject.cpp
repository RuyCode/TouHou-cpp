#include "core/GameObject.h"
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

GameObject::GameObject(glm::vec2 position, std::shared_ptr<Collider> collider)
  : position(position),
    collider(collider),
    uuid(boost::uuids::random_generator()()),
    isActive(true) {

    std::cout << "Created object with id " << boost::uuids::to_string(uuid) << std::endl;
}

GameObject::GameObject(const GameObject& other)
  : position(other.position),
    collider(other.collider),
    uuid(other.uuid),
    isActive(other.isActive) {

    std::cout << "Created object with id " << boost::uuids::to_string(uuid) << std::endl;
}

bool GameObject::operator==(const GameObject& other) const {
    return uuid == other.uuid;
}

std::shared_ptr<Collider> GameObject::GetCollider() {
  return collider;
}

void GameObject::SetCollider(std::shared_ptr<Collider> collider) {
  this->collider = collider;
}

glm::vec2 GameObject::GetPosition() {
  return position;
}

void GameObject::SetPosition(glm::vec2 pos) {
  position = pos;
}

GameObject::~GameObject() {
  std::cout << "Deleted object with id " << boost::uuids::to_string(uuid) << std::endl;
}