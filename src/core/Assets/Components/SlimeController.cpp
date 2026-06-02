//
// Created by Antonin Fruchet on 02/06/2026.
//

#include "SlimeController.h"

#include "Assets/GameObject.h"

SlimeController::SlimeController()
{
}

void SlimeController::Start()
{
    Component::Start();
    animator = gameObject->getComponent<Animator>();
}

void SlimeController::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
}

void SlimeController::takeDamage()
{
    animator->setParam("takeDamage", true);
}
