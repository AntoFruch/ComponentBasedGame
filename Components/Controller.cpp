//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "Controller.h"

#include <iostream>

#include "GameObject.h"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Input/InputManager.h"
#include <string>

#include "SlimeController.h"

Controller::Controller(const sf::Vector2f& pos, const sf::Vector2f& dimensions)
: CharacterController(pos, dimensions)
{}

Controller::~Controller()
{
}

void slashCallback(const std::vector<Collider*>& hits)
{
    for (const auto& hit : hits)
    {
        std::cout << "hit collider :" << hit->gameObject->getLabel() << std::endl;
        if (auto slime = hit->gameObject->getComponent<SlimeController>())
        {
            std::cout << "damage slime" << std::endl;
            slime->takeDamage();
        }
    }
}

void Controller::Start()
{
    CharacterController::Start();
    moveAction = InputManager::findAction("Move");
    slashAction = InputManager::findAction("Slash");
    animator = gameObject->getComponent<Animator>();
}

void Controller::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    handleMovement(elapsedTime);
    slash(elapsedTime);
}

void Controller::handleMovement(const sf::Time& elapsedTime)
{
    const sf::Vector2f rawDirection = moveAction->ReadValue<sf::Vector2f>();
    auto direction = rawDirection != sf::Vector2f{0,0} ?
    rawDirection.normalized() : sf::Vector2f{0,0};

    // remember facing direction for actions (use last non-zero direction)
    if (direction != sf::Vector2f{0,0}) {
        facing = direction;
    }

    move(direction*speed*elapsedTime.asSeconds()); // from character controller

    if (direction == sf::Vector2f{0,0})
    {
        animator->setParam("moving", false);
    } else
    {
        animator->setParam("moving", true);
        animator->setParam("forwardWalk", rawDirection.y);
        animator->setParam("sideWalk", rawDirection.x);
    }
}

void Controller::slash(const sf::Time& elapsedTime)
{
    if (slashAction->wasPerformedThisFrame())
    {
        animator->setParam("slash", true);

        // Create a temporary trigger collider in front of the player to represent a sword swing
        Collider* playerCollider = gameObject->getComponent<Collider>();
        if (!playerCollider) return;

        // Compute origin bounds and sizes
        const sf::FloatRect bounds = playerCollider->getBounds();
        const float ow = bounds.size.x;
        const float oh = bounds.size.y;
        const sf::Vector2f size = {ow, oh};

        // Use CollisionsManager helper to compute the placement of a collider in the facing direction.
        // createRelativeCollider returns a unique_ptr<Collider> positioned in world space (not started/registered).
        auto probe = CollisionsManager::createRelativeCollider(*playerCollider, facing, size, true);
        const sf::FloatRect probeBounds = probe->getBounds();
        const sf::Vector2f newCenter{probeBounds.position.x + probeBounds.size.x / 2.f, probeBounds.position.y + probeBounds.size.y / 2.f};

        // Create a child GameObject positioned so its world position == newCenter
        const sf::Vector2f parentWorldPos = gameObject->transform.getWorldPosition();
        const sf::Vector2f childLocalPos = {newCenter.x - parentWorldPos.x, newCenter.y - parentWorldPos.y};

        static int slashCounter = 0;
        const std::string label = "slash_" + std::to_string(++slashCounter);

        auto child = std::make_unique<GameObject>(label, childLocalPos, sf::degrees(0.f), sf::Vector2f{1.f,1.f}, true);

        auto swingCollider = std::make_unique<Collider>(sf::Vector2f{0.f,0.f}, size, true);
        swingCollider->setTriggerCallback(slashCallback);
        child->addComponent(std::move(swingCollider));

        // Add child to player so it gets cleaned up with the player; register/start it immediately
        gameObject->addChild(std::move(child));
        GameObject* added = gameObject->getChild(label);
        if (added)
        {
            added->start();
            added->update(sf::Time::Zero);
        }
    }
}
