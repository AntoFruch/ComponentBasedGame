//
// Created by Antonin Fruchet on 22/06/2026.
//
#include "Component.h"

void Component::Start()
{
}

void Component::Update(const sf::Time& elapsedTime)
{
}

void Component::setParent(GameObject* go)
{
    gameObject = go;
}
