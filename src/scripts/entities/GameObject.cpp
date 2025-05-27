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

std::shared_ptr<Collider> GameObject::GetCollider() const {
    return collider;
}

void GameObject::SetCollider(std::shared_ptr<Collider> collider) {
    this->collider = collider;
}

glm::vec2 GameObject::GetLocalPosition() const {
    return position;
}

glm::vec2 GameObject::GetWorldPosition() const {
    glm::vec2 worldPos = position;
    std::weak_ptr<GameObject> currentParent = parent;

    while (std::shared_ptr<GameObject> p = currentParent.lock()) {
        worldPos += p->position;
        currentParent = p->parent;
    }

    return worldPos;
}


void GameObject::SetLocalPosition(glm::vec2 pos) {
    position = pos;
}

void GameObject::SetWorldPosition(glm::vec2 pos) {
    if (std::shared_ptr<GameObject> p = parent.lock()) {
        position = pos - p->GetWorldPosition();
    } else {
        position = pos;
    }
}


void GameObject::SetParent(std::shared_ptr<GameObject> newParent) {
    if (!newParent) {
        parent.reset();
        return;
    }

    if (newParent.get() == this) {
        Logger::Instance().Error("Cannot set self as a parent (UUID: ", boost::uuids::to_string(uuid), ")");
        return;
    }

    std::shared_ptr<GameObject> current = newParent;
    while (current) {
        if (current.get() == this) {
            Logger::Instance().Error("Circular parenting (UUID: ", boost::uuids::to_string(uuid), ")");
            return;
        }
        current = current->parent.lock();
    }

    glm::vec2 worldPos = GetWorldPosition();

    parent = newParent;

    if (std::shared_ptr<GameObject> p = parent.lock()) {
        SetLocalPosition(worldPos - p->GetWorldPosition());
    } else {
        SetLocalPosition(worldPos);
    }
}

boost::uuids::uuid GameObject::GetUUID() const {
    return uuid;
}

void GameObject::DrawHitbox(sf::RenderTarget& target, sf::Color color) {
    if (auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(collider)) {
        sf::CircleShape col;

    float radius = circleCollider->GetRadius();

        col.setRadius(radius);
        col.setOrigin({radius, radius});
        glm::vec2 colCentrePos = GetWorldPosition() + circleCollider->GetOffset();
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