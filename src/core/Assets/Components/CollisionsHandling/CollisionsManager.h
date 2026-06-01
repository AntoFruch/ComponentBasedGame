//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#define COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "Collider.h"

class CollisionsManager {
    static std::vector<Collider*> hitboxes;

public:
    static void registerCollider(Collider*);
    static void unRegisterCollider(const Collider*);
    static sf::Vector2f move(Collider& collider, const sf::Vector2f& delta);
};



#endif //COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
