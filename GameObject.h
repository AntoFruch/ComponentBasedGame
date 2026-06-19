//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMEOBJECT_H
#define COMPONENT_BASED_ARCH_GAMEOBJECT_H
#include <memory>
#include <vector>

#include "Transform.h"
#include "Components/Component.h"
#include "exceptions/IllegalOperationException.h"


class GameObject
{
    std::string label;
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<GameObject>> children;

    bool active;
    bool waitingDestruction{false};

public:
    Transform transform;

    explicit GameObject(std::string_view label, const sf::Vector2f& position, const sf::Angle& rotation,
                        const sf::Vector2f& scale, bool active);

    void start();
    void update(const sf::Time& elapsedTime);

    void addComponent(std::unique_ptr<Component> c);
    template <typename T>
    [[nodiscard]] T* getComponent() const;
    template <typename T>
    [[nodiscard]] std::vector<T*> getComponents() const;

    void addChild(std::unique_ptr<GameObject> child);
    [[nodiscard]] std::vector<GameObject*> getChildren() const;
    [[nodiscard]] GameObject* getChild(std::string_view label) const;

    [[nodiscard]] std::string_view getLabel() const;
    void setLabel(const std::string&);
    void setActive(bool);
    [[nodiscard]]bool isActive() const;

    [[nodiscard]] bool isWaitingDestruction() const;
    void destroySelf();
};

template <typename T>
T* GameObject::getComponent() const
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    for (const auto& component : components)
    {
        auto* target = dynamic_cast<T*>(component.get());
        if (target != nullptr)
        {
            return target;
        }
    }
    return nullptr;
}

template <typename T>
std::vector<T*> GameObject::getComponents() const
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
    std::vector<T*> list{};
    for (const auto& component : components)
    {
        auto* target = dynamic_cast<T*>(component.get());
        if (target != nullptr)
        {
            list.push_back(target);
        }
    }
    return list;
}


#endif //COMPONENT_BASED_ARCH_GAMEOBJECT_H
