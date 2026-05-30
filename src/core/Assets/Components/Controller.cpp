//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "Controller.h"

#include <iostream>

#include "Assets/GameObject.h"
#include "InputManagement/InputManager.h"

Controller::~Controller()
{
}

void Controller::Start()
{
    Component::Start();
    moveAction = InputManager::findAction("Move");
    slashAction = InputManager::findAction("Slash");
    wandAction = InputManager::findAction("Wand");
    bowAction = InputManager::findAction("Bow");
    hitAction = InputManager::findAction("Hit");
    animator = gameObject->getComponent<Animator>();
}

void Controller::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    move(elapsedTime);
    slash(elapsedTime);
    wand(elapsedTime);
    bow(elapsedTime);
    hit(elapsedTime);
}

void Controller::move(const sf::Time& elapsedTime)
{
    const sf::Vector2f rawDirection = moveAction->ReadValue<sf::Vector2f>();
    auto direction = rawDirection != sf::Vector2f{0,0} ?
    rawDirection.normalized() : sf::Vector2f{0,0};
    gameObject->transform.move(direction*speed*elapsedTime.asSeconds());
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
    }
}

void Controller::wand(const sf::Time& elapsedTime)
{
    if (wandAction->wasPerformedThisFrame())
    {
        animator->setParam("wand", true);
    }
}

void Controller::bow(const sf::Time& elapsedTime)
{
    if (bowAction->wasPerformedThisFrame())
    {
        animator->setParam("bow", true);
    }
}

void Controller::hit(const sf::Time& elapsedTime)
{
    if (hitAction->wasPerformedThisFrame())
    {
        animator->setParam("hit", true);
    }
}
