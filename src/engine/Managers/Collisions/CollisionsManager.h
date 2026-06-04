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
    // Create a new Collider positioned relative to an existing one.
    // - origin: the collider to position relative to
    // - direction: a vector indicating direction (e.g. {1,0} right, {0,1} down, {1,1} diagonal)
    // - dimensions: if {0,0} (default) the origin's dimensions are used
    // - trigger: whether the created collider is a trigger
    // Returns a unique_ptr owning the newly created Collider. The created collider's hitbox
    // position is set to world coordinates corresponding to the requested placement.
    static std::unique_ptr<Collider> createRelativeCollider(const Collider& origin, const sf::Vector2f& direction, const sf::Vector2f& dimensions = {0.f, 0.f}, bool trigger = false);
};

#endif //COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
