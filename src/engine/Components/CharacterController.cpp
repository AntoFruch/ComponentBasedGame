//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CharacterController.h"

#include "GameObject.h"
#include "Managers/Collisions/CollisionsManager.h"

CharacterController::CharacterController(const sf::Vector2f& pos, const sf::Vector2f& dimensions)
    : colliderPos(pos), colliderSize(dimensions)
{
}

void CharacterController::Start()
{
    Component::Start();
    auto ptr = std::make_unique<Collider>(colliderPos, colliderSize, false);
    collider = ptr.get();
    gameObject->addComponent(std::move(ptr));
}

void CharacterController::move(const sf::Vector2f& delta)
{
    auto newDelta = CollisionsManager::move(*collider, delta);
    gameObject->transform.move(newDelta);
    collider->syncWithTransform();
}
