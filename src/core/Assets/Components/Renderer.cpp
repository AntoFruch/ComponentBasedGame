//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Renderer.h"

#include "Assets/GameObject.h"

Renderer::Renderer()
{
    Renderer::Start();
}
void Renderer::Start()
{
    //TESTING
    mShape = std::make_unique<sf::CircleShape>(30);
}

void Renderer::Update(const sf::Time& elapsedTime)
{
    mShape->setPosition(gameObject->transform.getPosition());
    mShape->setRotation(gameObject->transform.getRotation());
    mShape->setScale(gameObject->transform.getScale());
}

void Renderer::render(sf::RenderWindow& window) const
{
    window.draw(*mShape);
}
