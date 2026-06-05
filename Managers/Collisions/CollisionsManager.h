//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#define COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "Components/Collider.h"
#include <memory>

class CollisionsManager {
    static std::vector<Collider*> solidColliders;
    static std::vector<Collider*> triggersColliders;

public:
    static void registerCollider(Collider*);
    static void unRegisterCollider(const Collider*);
    static sf::Vector2f move(Collider& collider, const sf::Vector2f& delta);
    static std::vector<Collider*> checkTrigger(Collider& collider);

    static void debugDraw(sf::RenderWindow& window);
};

#endif //COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H


