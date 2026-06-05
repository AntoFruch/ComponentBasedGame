//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CharacterController.h"

#include <iostream>

#include "GameObject.h"
#include "pugixml.hpp"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Scene/ComponentFactory.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("CharacterController", [](const pugi::xml_node& node) {
            return std::make_unique<CharacterController>(
                sf::Vector2f{
                    node.attribute("colliderX").as_float(),
                    node.attribute("colliderY").as_float()
                },
                sf::Vector2f{
                    node.attribute("colliderW").as_float(),
                    node.attribute("colliderH").as_float()
                }
                );
        });
        return true;
    }();
}
// --------------------------



CharacterController::CharacterController(const sf::Vector2f& colliderPos, const sf::Vector2f& colliderSize)
    : colliderPos(colliderPos), colliderSize(colliderSize)
{
}

void CharacterController::Start()
{
    Component::Start();
    std::cout << "Starting CharacterController" << std::endl;
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
