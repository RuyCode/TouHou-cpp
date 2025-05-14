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

    void SetCollider(std::shared_ptr<Collider> collider);

    glm::vec2 GetPosition();

    void SetPosition(glm::vec2 pos);

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void Draw(sf::RenderTarget& window) = 0;

    ~GameObject();
};

#endif // GAMEOBJECT_H