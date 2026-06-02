//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CollisionsManager.h"

#include "exceptions/IllegalOperationException.h"
#include <memory>

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
        if (other->getBounds().findIntersection(collider.getBounds()))
        {
            hitColliders.push_back(other);
        }
    }
    return hitColliders;
}

std::unique_ptr<Collider> CollisionsManager::createRelativeCollider(const Collider& origin, const sf::Vector2f& direction, const sf::Vector2f& dimensions, bool trigger)
{
    // Get origin world bounds
    const auto bounds = origin.getBounds();
    const float ow = bounds.size.x;
    const float oh = bounds.size.y;

    // Determine size of new collider: if user provided {0,0}, use origin size
    const sf::Vector2f size = (dimensions.x == 0.f && dimensions.y == 0.f) ? sf::Vector2f{ow, oh} : dimensions;

    // Normalize direction to signs (-1, 0, 1) per axis
    const auto sign = [](float v) -> float { return v > 0.f ? 1.f : (v < 0.f ? -1.f : 0.f); };
    const float sx = sign(direction.x);
    const float sy = sign(direction.y);

    // Compute offset so the new collider sits just adjacent to the origin without overlap
    const float offsetX = sx * (ow / 2.f + size.x / 2.f);
    const float offsetY = sy * (oh / 2.f + size.y / 2.f);

    // Compute new center in world coordinates
    const sf::Vector2f originCenter{bounds.position.x + ow / 2.f, bounds.position.y + oh / 2.f};
    const sf::Vector2f newCenter{originCenter.x + offsetX, originCenter.y + offsetY};

    // Construct collider using newCenter as its position and set the hitbox world position
    auto collider = std::make_unique<Collider>(newCenter, size, trigger);
    collider->setPosition(newCenter);
    return collider;
}

