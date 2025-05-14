#include "core/GameObject.h"

#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <memory>

template <typename T>
class ObjectPool {
private:
    std::vector<std::shared_ptr<T>> pool;

    std::shared_ptr<T> createObject(bool isActiveByDefault = false) {
        std::shared_ptr<T> createdObject = std::make_shared<T>();
        createdObject->SetActive(isActiveByDefault);

        pool.push_back(createdObject);
        return createdObject;
    }
public:
    ObjectPool(int count) {
        pool.reserve(count);
        for (int i = 0; i < count; ++i) {
            createObject();
        }
    }

    bool HasFreeElement(std::shared_ptr<T>& element) {
        for (const std::shared_ptr<T>& obj : pool) {
            if (!obj->GetActive()) {
                element = obj;
                return true; 
            }
        }

        element = nullptr;
        return false;
    }

    std::shared_ptr<T> GetFreeElement() {
        std::shared_ptr<T> element = nullptr;

        if (HasFreeElement(element)) {
            element->SetActive(true);
            return element;
        }

        return createObject();
    }

    std::vector<std::shared_ptr<T>>& GetPool() {
        return pool;
    }
};

#endif // OBJECT_POOL_H