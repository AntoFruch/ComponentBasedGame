//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Component.h"

Component::Component()
{

}

void Component::Start()
{
    /* if something has to be done here */
}

void Component::Update(const sf::Time& elapsedTime)
{
    /* if something has to be done here */
}

void Component::setParent(GameObject* go)
{
    gameObject=go;
}

