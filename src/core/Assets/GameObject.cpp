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
    for (const auto& child : children)
    {
        child->start();
    }
}

void GameObject::update(const sf::Time& elapsedTime) {
    for (auto& c : components)
    {
        c->Update(elapsedTime);
    }
    for (auto& child : children)
    {
        child->update(elapsedTime);
    }
}

void GameObject::addComponent(std::unique_ptr<Component> c)
{
    c->setParent(this);
    components.push_back(std::move(c));
}

void GameObject::addChild(std::unique_ptr<GameObject> child)
{
    transform.addChild(&child->transform);
    children.push_back(std::move(child));
}
