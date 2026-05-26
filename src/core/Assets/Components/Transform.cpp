//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Transform.h"

Transform::Transform(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, Transform* parent)
    : position(position), rotation(rotation), scale(scale), parent(parent)
{}

const std::vector<Transform>& Transform::getChildren() const
{
    return children;
}

const sf::Vector2f& Transform::getPosition() const
{
    return position;
}

const sf::Angle& Transform::getRotation() const
{
    return rotation;
}

const sf::Vector2f& Transform::getScale() const
{
    return scale;
}

void Transform::set_position(const sf::Vector2f& position)
{
    this->position = position;
}

void Transform::set_rotation(const sf::Angle& rotation)
{
    this->rotation = rotation;
}

void Transform::set_scale(const sf::Vector2f& scale)
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
