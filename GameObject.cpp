//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "src/engine/GameObject.h"
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
    for (const auto& child : children)
    {
        child->start();
    }
}

void GameObject::update(const sf::Time& elapsedTime) {

    if (!active) return;
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

std::vector<GameObject*> GameObject::getChildren() const
{
    std::vector<GameObject*> vector;
    for (const auto& child : children)
    {
        vector.push_back(child.get());
    }
    return vector;
}

GameObject* GameObject::getChild(std::string_view label) const
{
    for (const auto& child : children)
    {
        if (child->getLabel() == label)
        {
            return child.get();
        }
    }
    return nullptr;
}

std::string_view GameObject::getLabel() const
{
    return label;
}

void GameObject::setActive(bool active)
{
    this->active = active;
}
