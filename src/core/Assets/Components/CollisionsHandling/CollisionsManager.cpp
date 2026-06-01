//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CollisionsManager.h"

std::vector<Collider*> CollisionsManager::hitboxes{};

void CollisionsManager::registerCollider(Collider* collider)
{
    hitboxes.push_back(collider);
}

void CollisionsManager::unRegisterCollider(const Collider* collider)
{
    std::erase_if(hitboxes, [&collider](Collider const* c) { return c == collider; });
}

sf::Vector2f CollisionsManager::move(Collider& collider, const sf::Vector2f& delta)
{
    const auto collidesWithAnyOther = [&collider]
    {
        for (const auto& other : hitboxes)
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
