//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMEOBJECT_H
#define COMPONENT_BASED_ARCH_GAMEOBJECT_H
#include <memory>
#include <type_traits>
#include <vector>

#include "Components/Component.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"


class GameObject {
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<GameObject>> children;

public:
    Transform transform;

    explicit GameObject(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale);

    void start();
    void update(const sf::Time& elapsedTime);
    void addComponent(std::unique_ptr<Component> c);
    void addChild(std::unique_ptr<GameObject> child);
    template<typename T>
    T* getComponent();
};

template <typename T>
T* GameObject::getComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    for (const auto& component : components) {
        T* target = dynamic_cast<T*>(component.get());
        if (target != nullptr) {
            return target;
        }
    }
    return nullptr;
}



#endif //COMPONENT_BASED_ARCH_GAMEOBJECT_H
