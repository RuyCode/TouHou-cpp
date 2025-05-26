#include "scripts/entities/GameObject.h"
#include "scripts/colliders/CircleCollider.h"
#include "scripts/colliders/RectangleCollider.h"
#include "scripts/utils/Logger.h"
#include "scripts/utils/Debug.h"

#include <boost/uuid/uuid_io.hpp>

GameObject::GameObject(glm::vec2 position, std::shared_ptr<Collider> collider)
  : position(position),
    collider(collider),
    uuid(boost::uuids::random_generator()()),
    isActive(true) {

    Logger::Instance().Info("Created object with id ", boost::uuids::to_string(uuid));
}

GameObject::GameObject(const GameObject& other)
  : position(other.position),
    collider(other.collider),
    uuid(other.uuid),
    isActive(other.isActive) {

    Logger::Instance().Info("Created object with id ", boost::uuids::to_string(uuid));
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

boost::uuids::uuid GameObject::GetUUID() {
  return uuid;
}

bool GameObject::operator==(const GameObject& other) {
  return uuid == other.uuid;
}

void GameObject::DrawHitbox(sf::RenderTarget& target, sf::Color color) {
  if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(collider)) {
    sf::CircleShape col;

    float radius = circleCollider->GetRadius();

    col.setRadius(radius);
    col.setOrigin({radius, radius});
    glm::vec2 colCentrePos = position + circleCollider->GetOffset();
    col.setPosition({colCentrePos.x, colCentrePos.y});
    
    col.setOutlineColor(color);
    col.setOutlineThickness(2);
    col.setFillColor(sf::Color::Transparent);

    target.draw(col);
  } else if (auto rectangleCollider = std::dynamic_pointer_cast<RectangleCollider>(collider)) {

  }
}

GameObject::~GameObject() {
  Logger::Instance().Info("Deleted object with id ", boost::uuids::to_string(uuid));
}