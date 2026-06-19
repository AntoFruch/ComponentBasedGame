//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "Collider.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "Managers/Collisions/CollisionsManager.h"
#include "GameObject.h"

Collider::Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger) : localPos(pos), trigger(trigger)
{
    hitbox = sf::RectangleShape();
    hitbox.setSize(dimensions);
    hitbox.setOrigin(dimensions / 2.f);

    // DEBUGGING
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(trigger ? sf::Color::Red : sf::Color::Green);
    hitbox.setOutlineThickness(0.5);
}

Collider::~Collider()
{
    CollisionsManager::unRegisterCollider(this);
}

void Collider::Start()
{
    Component::Start();
    CollisionsManager::registerCollider(this);
    syncWithTransform();
}

void Collider::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    syncWithTransform();

    if (!isTrigger()) return;

    if (const auto& hitColliders = CollisionsManager::checkTrigger(*this); !hitColliders.empty())
    {
        if (callback)
        {
            callback(hitColliders, this);
        }
    }
}

bool Collider::isTrigger() const
{
    return trigger;
}

void Collider::syncWithTransform()
{
    const sf::Vector2f scale = gameObject->transform.getWorldScale();

    setPosition(gameObject->transform.getWorldPosition() +
        Transform::rotateVector(
            sf::Vector2f{localPos.x*scale.x, localPos.y*scale.y},
            gameObject->transform.getWorldRotation())
        );

    hitbox.setRotation(gameObject->transform.getWorldRotation());
    hitbox.setScale(scale);
}

void Collider::setPosition(sf::Vector2f pos)
{
    hitbox.setPosition(pos);
}

void Collider::setSize(sf::Vector2f size)
{
    hitbox.setSize(size);
    // Keep origin centered when size changes so position stays aligned with GameObject
    hitbox.setOrigin(size / 2.f);
}

void Collider::move(sf::Vector2f delta)
{
    hitbox.move(delta);
}

sf::Vector2f Collider::getPosition()
{
    return hitbox.getPosition();
}

sf::Vector2f Collider::getSize()
{
    return hitbox.getSize();
}

sf::FloatRect Collider::getBounds() const
{
    return hitbox.getTransform().transformRect(sf::FloatRect({0.f, 0.f}, hitbox.getSize()));
}

std::array<sf::Vector2f, 4> Collider::getWorldCorners() const
{
    const sf::Transform transform = hitbox.getTransform();
    const sf::Vector2f size = hitbox.getSize();

    return {
        transform.transformPoint({0.f, 0.f}),
        transform.transformPoint({size.x, 0.f}),
        transform.transformPoint({size.x, size.y}),
        transform.transformPoint({0.f, size.y})
    };
}

bool Collider::intersects(const Collider& other) const
{
    const auto firstCorners = getWorldCorners();
    const auto secondCorners = other.getWorldCorners();

    const auto dot = [](const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x * b.x + a.y * b.y;
    };

    const auto perpendicular = [](const sf::Vector2f& axis)
    {
        return sf::Vector2f{-axis.y, axis.x};
    };

    const auto project = [&dot](const std::array<sf::Vector2f, 4>& corners, const sf::Vector2f& axis)
    {
        float min = dot(corners[0], axis);
        float max = min;

        for (const sf::Vector2f& corner : corners)
        {
            const float projection = dot(corner, axis);
            min = std::min(min, projection);
            max = std::max(max, projection);
        }

        return std::pair{min, max};
    };

    const std::array axes{
        firstCorners[1] - firstCorners[0],
        firstCorners[3] - firstCorners[0],
        secondCorners[1] - secondCorners[0],
        secondCorners[3] - secondCorners[0]
    };

    for (const sf::Vector2f& edge : axes)
    {
        const sf::Vector2f axis = perpendicular(edge);
        if (axis.x == 0.f && axis.y == 0.f)
        {
            continue;
        }

        const auto [firstMin, firstMax] = project(firstCorners, axis);
        const auto [secondMin, secondMax] = project(secondCorners, axis);

        if (firstMax <= secondMin || secondMax <= firstMin)
        {
            return false;
        }
    }

    return true;
}

void Collider::setTriggerCallback(std::function<void(const std::vector<Collider*>&, Collider*)> callback)
{
    this->callback = std::move(callback);
}

void Collider::debugDraw(sf::RenderWindow& window)
{
    window.draw(hitbox);
}
