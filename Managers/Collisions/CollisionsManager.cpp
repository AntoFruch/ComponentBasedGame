//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "Managers/Collisions/CollisionsManager.h"

#include "exceptions/IllegalOperationException.h"
#include <memory>

#include "GameObject.h"

std::vector<Collider*> CollisionsManager::solidColliders{};
std::vector<Collider*> CollisionsManager::triggersColliders{};

void CollisionsManager::registerCollider(Collider* collider)
{
    if (!collider->isTrigger())
    {
        solidColliders.push_back(collider);
        return;
    }
    triggersColliders.push_back(collider);
}

void CollisionsManager::unRegisterCollider(const Collider* collider)
{
    if (!collider->isTrigger())
    {
        std::erase_if(solidColliders, [&collider](Collider const* c) { return c == collider; });
        return;
    }
    std::erase_if(triggersColliders, [&collider](Collider const* c) { return c == collider; });
}

sf::Vector2f CollisionsManager::move(Collider& collider, const sf::Vector2f& delta)
{
    if (collider.isTrigger()) return delta;
    const auto collidesWithAnyOther = [&collider]
    {
        for (const auto& other : solidColliders)
        {
            if (other->isTrigger() || other == &collider) continue;

            if (auto intersection = collider.getBounds().findIntersection(other->getBounds());
                intersection && intersection->size.x > 0.f && intersection->size.y > 0.f)
            {
                return true;
            }
        }

        return false;
    };

    const auto resolveAxis = [&collider, &collidesWithAnyOther](const sf::Vector2f& axisDelta)
    {
        if (axisDelta.x == 0.f && axisDelta.y == 0.f)
        {
            return 0.f;
        }

        collider.move(axisDelta);
        if (collidesWithAnyOther())
        {
            collider.move({-axisDelta.x, -axisDelta.y});
            return 0.f;
        }

        return axisDelta.x != 0.f ? axisDelta.x : axisDelta.y;
    };

    const sf::Vector2f resolved{
        resolveAxis({delta.x, 0.f}),
        resolveAxis({0.f, delta.y}),
    };

    collider.move({-resolved.x, -resolved.y});
    return resolved;
}

std::vector<Collider*> CollisionsManager::checkTrigger(Collider& collider)
{
    if (!collider.isTrigger()) throw IllegalOperationException("checkTrigger() argument must be a trigger type collider");

    std::vector<Collider*> hitColliders;
    for (const auto& other : solidColliders)
    {
        if (other->getBounds().findIntersection(collider.getBounds()) && other->gameObject->isActive())
        {
            hitColliders.push_back(other);
        }
    }
    return hitColliders;
}

void CollisionsManager::debugDraw(sf::RenderWindow& window)
{
    for (const auto& col:solidColliders)
    {
        col->debugDraw(window);
    }
    for (const auto& col:triggersColliders)
    {
        col->debugDraw(window);
    }
}



