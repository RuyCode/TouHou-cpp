#include "scripts/colliders/Collider.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class GameObject {
protected:
    boost::uuids::uuid uuid;
    bool isActive;

    glm::vec2 position;
    std::shared_ptr<Collider> collider;
    std::weak_ptr<GameObject> parent;

    GameObject(glm::vec2 position, std::shared_ptr<Collider> collider);

public:
    GameObject(const GameObject& other);

    void SetActive(bool isActive) {
        this->isActive = isActive;
    }

    bool GetActive() {
        return isActive;
    }

    bool operator==(const GameObject& other) const;

    std::shared_ptr<Collider> GetCollider() const;

    void SetCollider(std::shared_ptr<Collider> collider);

    glm::vec2 GetLocalPosition() const;

    void SetLocalPosition(glm::vec2 pos);

    glm::vec2 GetWorldPosition() const;

    void SetWorldPosition(glm::vec2 pos);

    void SetParent(std::shared_ptr<GameObject> newParent);

    boost::uuids::uuid GetUUID() const;

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void Draw(sf::RenderTarget& target) = 0;

    void DrawHitbox(sf::RenderTarget& target, sf::Color color);

    ~GameObject();
};

#endif // GAMEOBJECT_H