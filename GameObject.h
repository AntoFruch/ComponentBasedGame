//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMEOBJECT_H
#define COMPONENT_BASED_ARCH_GAMEOBJECT_H
#include <memory>
#include <vector>

#include "Transform.h"
#include "Components/Component.h"


class GameObject {
    std::string label;
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<GameObject>> children;

    bool active;
    bool waitingDestruction{false};
public:
    Transform transform;

    explicit GameObject(std::string_view label, const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, bool active);

    void start();
    void update(const sf::Time& elapsedTime);
    void addComponent(std::unique_ptr<Component> c);
    template<typename T>
    T* getComponent();

    void addChild(std::unique_ptr<GameObject> child);
    std::vector<GameObject*> getChildren() const;
    GameObject* getChild(std::string_view label) const;

    std::string_view getLabel() const;
    void setActive(bool);

    bool isWaitingDestruction() const;
    void destroySelf();
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
