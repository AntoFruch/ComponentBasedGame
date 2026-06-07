//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "Collider.h"

#include <iostream>

#include "Managers/Collisions/CollisionsManager.h"
#include "GameObject.h"
#include "Managers/Scene/ComponentFactory.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("Collider", [](const pugi::xml_node& node) {
            return std::make_unique<Collider>(
                sf::Vector2f{
                    node.attribute("x").as_float(),
                    node.attribute("y").as_float(),
                },
                sf::Vector2f{
                    node.attribute("width").as_float(),
                    node.attribute("height").as_float(),
                },
                node.attribute("trigger").as_bool()
                );
        });
        return true;
    }();
}
// --------------------------



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
    if (gameObject->getLabel() == "X+")
    {
        //std::cout << scale.x << " " << scale.y << " " << localPos.x << " " << localPos.y << std::endl;
    }

    setPosition({
        gameObject->transform.getWorldPosition().x + localPos.x*scale.x,
        gameObject->transform.getWorldPosition().y + localPos.y*scale.y
    });
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

void Collider::setTriggerCallback(void(* callback)(const std::vector<Collider*>&, Collider*))
{
    this->callback = callback;
}

void Collider::debugDraw(sf::RenderWindow& window)
{
    window.draw(hitbox);
}

