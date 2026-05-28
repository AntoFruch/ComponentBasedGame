//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "Controller.h"

#include <iostream>

#include "Assets/GameObject.h"
#include "InputManagement/InputManager.h"

Controller::Controller()
{
}

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
    animator = gameObject->getComponent<Animator>();
}

void Controller::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    move(elapsedTime);
    slash(elapsedTime);
    wand(elapsedTime);
    bow(elapsedTime);
}

void Controller::move(const sf::Time& elapsedTime)
{
    auto direction = moveAction->ReadValue<sf::Vector2f>() != sf::Vector2f{0,0} ?
    moveAction->ReadValue<sf::Vector2f>().normalized() : sf::Vector2f{0,0};
    gameObject->transform.move(direction*speed*elapsedTime.asSeconds());
    if (direction == sf::Vector2f{0,0})
    {
        animator->setParam("moving", false);
    } else
    {
        animator->setParam("moving", true);
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