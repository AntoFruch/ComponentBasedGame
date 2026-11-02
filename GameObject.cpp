//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "GameObject.h"
GameObject::GameObject(std::string_view label, const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, bool active)
    : label(label), transform(position, rotation, scale, nullptr), active(active)
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

    if (!active) return;
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

std::string_view GameObject::getLabel() const
{
    return label;
}
void GameObject::setLabel(const std::string& label)
{
    this->label= label;
}

void GameObject::setActive(bool active)
{
    this->active = active;
}

bool GameObject::isActive() const
{
    return active;
}

bool GameObject::isWaitingDestruction() const
{
    return waitingDestruction;
}

void GameObject::destroySelf()
{
    waitingDestruction = true;
}

