//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "Controller.h"

#include "Assets/GameObject.h"
#include "InputManagement/InputManager.h"

Controller::Controller()
{
    Controller::Start();
}

Controller::~Controller()
{
}

void Controller::Start()
{
    Component::Start();
    moveAction = InputManager::findAction("Move");
}

void Controller::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    auto direction = moveAction->ReadValue<sf::Vector2f>() != sf::Vector2f{0,0} ?
    moveAction->ReadValue<sf::Vector2f>().normalized() : sf::Vector2f{0,0};
    gameObject->transform.move(direction*speed*elapsedTime.asSeconds());
}
