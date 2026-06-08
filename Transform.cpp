//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Transform.h"

#include <iostream>

Transform::Transform(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, Transform* parent)
    : position(position), rotation(rotation), scale(scale), parent(parent)
{}

const std::vector<Transform*>& Transform::getChildren() const
{
    return children;
}

void Transform::addChild(Transform* child)
{
    child->setParent(this);
    children.push_back(child);
}

void Transform::setParent(Transform* parent)
{
    this->parent = parent;
}

const sf::Vector2f& Transform::getLocalPosition() const
{
    return position;
}

const sf::Angle& Transform::getLocalRotation() const
{
    return rotation;
}

const sf::Vector2f& Transform::getLocalScale() const
{
    return scale;
}

sf::Vector2f Transform::getWorldPosition() const
{
    if (!parent) return position;
    const sf::Vector2f parentScale = parent->getWorldScale();
    const sf::Vector2f scaledLocal = { position.x * parentScale.x, position.y * parentScale.y };
    return parent->getWorldPosition() + scaledLocal;
}

sf::Angle Transform::getWorldRotation() const
{
    if (!parent) return rotation;
    return parent->getWorldRotation() + rotation;
}

sf::Vector2f Transform::getWorldScale() const
{
    if (!parent) return scale;
    const sf::Vector2f parentScale = parent->getWorldScale();
    return {parentScale.x * scale.x, parentScale.y * scale.y};
}

void Transform::setLocalPosition(const sf::Vector2f& position)
{
    this->position = position;
}

void Transform::setLocalRotation(const sf::Angle& rotation)
{
    this->rotation = rotation;
}

void Transform::setLocalScale(const sf::Vector2f& scale)
{
    this->scale = scale;
}

void Transform::move(sf::Vector2f amount)
{
    position+=amount;
}

void Transform::rotate(sf::Angle amount)
{
    rotation+=amount;
}

void Transform::rescale(sf::Vector2f amount)
{
    scale.x*=amount.x;
    scale.y*=amount.y;
}
