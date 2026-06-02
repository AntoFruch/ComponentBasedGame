//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "Collider.h"

#include <iostream>

#include "CollisionsManager.h"
#include "Assets/GameObject.h"

Collider::Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger) : dimensions(dimensions), localPos(pos), trigger(trigger)
{
    hitbox = sf::RectangleShape();
    hitbox.setSize(dimensions);
    hitbox.setOrigin(dimensions / 2.f);

    // DEBUGGING
    /*
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(0.5);
    */
}

Collider::~Collider()
{
    CollisionsManager::unRegisterCollider(this);
}

void Collider::Start()
{
    Component::Start();
    CollisionsManager::registerCollider(this);
}

void Collider::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    syncWithTransform();

    if (!isTrigger()) return;

    if (const auto& hitColliders = CollisionsManager::checkTrigger(*this); !hitColliders.empty())
    {
        callback(hitColliders);
    }
    gameObject->setActive(false);
}

bool Collider::isTrigger() const
{
    return trigger;
}

void Collider::syncWithTransform()
{
    const sf::Vector2f scale = gameObject->transform.getWorldScale();
    const sf::Vector2f scaledLocalPos{localPos.x * scale.x, localPos.y * scale.y};

    setPosition(gameObject->transform.getWorldPosition() + scaledLocalPos);
    hitbox.setRotation(gameObject->transform.getWorldRotation());
    hitbox.setScale(scale);
}

void Collider::setPosition(sf::Vector2f pos)
{
    hitbox.setPosition(pos);
}

void Collider::move(sf::Vector2f delta)
{
    hitbox.move(delta);
}

sf::FloatRect Collider::getBounds() const
{
    return hitbox.getTransform().transformRect(sf::FloatRect({0.f, 0.f}, dimensions));
}

void Collider::setTriggerCallback(void(* callback)(const std::vector<Collider*>&))
{
    this->callback = callback;
}

void Collider::debugDraw(sf::RenderWindow& window)
{
    window.draw(hitbox);
}

