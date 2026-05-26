//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMEOBJECT_H
#define COMPONENT_BASED_ARCH_GAMEOBJECT_H
#include <memory>
#include <vector>

#include "Components/Component.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"


class GameObject {
    std::vector<std::unique_ptr<Component>> components;

public:
    Transform transform;

    explicit GameObject(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale);

    void update(const sf::Time& elapsedTime);
    void addComponent(std::unique_ptr<Component> c);
};



#endif //COMPONENT_BASED_ARCH_GAMEOBJECT_H
