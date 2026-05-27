//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale)
    : transform(position, rotation, scale, nullptr)
{

}
void GameObject::start()
{
    for (const auto& c : components )
    {
        c->Start();
    }
}

void GameObject::update(const sf::Time& elapsedTime) {
    for (auto& c : components)
    {
        c->Update(elapsedTime);
    }
}

void GameObject::addComponent(std::unique_ptr<Component> c)
{
    c->setParent(this);
    components.push_back(std::move(c));
}
