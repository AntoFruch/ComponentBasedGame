//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CharacterController.h"

#include <iostream>

#include "GameObject.h"
#include "pugixml.hpp"
#include "Managers/Collisions/CollisionsManager.h"

CharacterController::CharacterController(const sf::Vector2f& colliderPos, const sf::Vector2f& colliderSize)
    : colliderPos(colliderPos), colliderSize(colliderSize)
{
}

void CharacterController::Start()
{
    Component::Start();
    auto ptr = std::make_unique<Collider>(colliderPos, colliderSize, false);
    collider = ptr.get();
    gameObject->addComponent(std::move(ptr));
    collider->Start();
}

void CharacterController::move(const sf::Vector2f& delta)
{
    auto newDelta = CollisionsManager::move(*collider, delta);
    gameObject->transform.move(newDelta);
}
