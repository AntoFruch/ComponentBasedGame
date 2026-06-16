//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CharacterController.h"

#include <iostream>

#include "GameObject.h"
#include "pugixml.hpp"
#include "exceptions/IllegalOperationException.h"
#include "Managers/Collisions/CollisionsManager.h"

void CharacterController::Start()
{
    Component::Start();
    collider = gameObject->getComponent<Collider>();

    if (!collider)
    {
        throw IllegalOperationException("CharacterController requires a Collider on the same GameObject");
    }
}

void CharacterController::move(const sf::Vector2f& delta)
{
    auto newDelta = CollisionsManager::move(*collider, delta);
    gameObject->transform.move(newDelta);
}
