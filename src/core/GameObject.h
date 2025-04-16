#include "core/Collider.h"

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

    GameObject(glm::vec2 position, std::shared_ptr<Collider> collider);

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void Draw(sf::RenderWindow& window) = 0;

public:
    GameObject(const GameObject& other);

    void SetActive(bool isActive) {
        this->isActive = isActive;
    }

    bool GetActive() {
        return isActive;
    }

    bool operator==(const GameObject& other) const;

    std::shared_ptr<Collider> GetCollider();

    glm::vec2 GetPosition();
};

#endif // GAMEOBJECT_H