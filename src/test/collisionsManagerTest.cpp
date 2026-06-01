#include <gtest/gtest.h>

#include "Assets/GameObject.h"
#include "Assets/Components/CollisionsHandling/Collider.h"
#include "Assets/Components/CollisionsHandling/CollisionsManager.h"

namespace {
Collider* addCollider(GameObject& gameObject, const sf::Vector2f& size)
{
    gameObject.addComponent(std::make_unique<Collider>(sf::Vector2f{0.f, 0.f}, size));
    gameObject.start();
    gameObject.update(sf::Time::Zero);
    return gameObject.getComponent<Collider>();
}
}

TEST(CollisionsManagerTest, DiagonalMovementAgainstHorizontalWallKeepsHorizontalSlide)
{
    GameObject player({0.f, 0.f}, sf::degrees(0.f), {1.f, 1.f});
    Collider* playerCollider = addCollider(player, {10.f, 10.f});

    GameObject wall({5.f, 10.f}, sf::degrees(0.f), {1.f, 1.f});
    addCollider(wall, {100.f, 10.f});

    const sf::Vector2f resolved = CollisionsManager::move(*playerCollider, {10.f, 10.f});

    EXPECT_FLOAT_EQ(resolved.x, 10.f);
    EXPECT_FLOAT_EQ(resolved.y, 0.f);
}

TEST(CollisionsManagerTest, DiagonalMovementAfterBeingBlockedFromAboveDoesNotEnterBlock)
{
    GameObject player({0.f, 0.f}, sf::degrees(0.f), {1.f, 1.f});
    Collider* playerCollider = addCollider(player, {10.f, 10.f});

    GameObject block({0.f, 10.f}, sf::degrees(0.f), {1.f, 1.f});
    addCollider(block, {100.f, 10.f});

    const sf::Vector2f blockedDown = CollisionsManager::move(*playerCollider, {0.f, 10.f});
    EXPECT_FLOAT_EQ(blockedDown.x, 0.f);
    EXPECT_FLOAT_EQ(blockedDown.y, 0.f);

    const sf::Vector2f diagonal = CollisionsManager::move(*playerCollider, {10.f, 10.f});
    EXPECT_FLOAT_EQ(diagonal.x, 10.f);
    EXPECT_FLOAT_EQ(diagonal.y, 0.f);
}

TEST(CollisionsManagerTest, DiagonalMovementAgainstVerticalWallKeepsVerticalSlide)
{
    GameObject player({0.f, 0.f}, sf::degrees(0.f), {1.f, 1.f});
    Collider* playerCollider = addCollider(player, {10.f, 10.f});

    GameObject wall({10.f, 5.f}, sf::degrees(0.f), {1.f, 1.f});
    addCollider(wall, {10.f, 100.f});

    const sf::Vector2f resolved = CollisionsManager::move(*playerCollider, {10.f, 10.f});

    EXPECT_FLOAT_EQ(resolved.x, 0.f);
    EXPECT_FLOAT_EQ(resolved.y, 10.f);
}

TEST(CollisionsManagerTest, UnblockedMovementIsUnchanged)
{
    GameObject player({0.f, 0.f}, sf::degrees(0.f), {1.f, 1.f});
    Collider* playerCollider = addCollider(player, {10.f, 10.f});

    GameObject wall({100.f, 100.f}, sf::degrees(0.f), {1.f, 1.f});
    addCollider(wall, {10.f, 10.f});

    const sf::Vector2f resolved = CollisionsManager::move(*playerCollider, {10.f, 10.f});

    EXPECT_FLOAT_EQ(resolved.x, 10.f);
    EXPECT_FLOAT_EQ(resolved.y, 10.f);
}
