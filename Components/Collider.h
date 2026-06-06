//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLIDER_H
#define COMPONENT_BASED_ARCH_COLLIDER_H
#include "Component.h"
#include "SFML/Graphics.hpp"

class
Collider : public Component {
    sf::RectangleShape hitbox;
    sf::Vector2f localPos;

    bool trigger;
    /**
     * @brief function ptr, the function is executed when a trigger collider triggers
     * must have arguments :
     * @param hits vector of pointers of hit targets
     * @param self pointer to the trigger
     */
    void (*callback)(const std::vector<Collider*>& hits, Collider* self);

public:
    Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger);
    ~Collider() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void setPosition(sf::Vector2f);
    void setSize(sf::Vector2f);
    void setTriggerCallback(void (*callback)(const std::vector<Collider*>&, Collider*));

    [[nodiscard]] bool isTrigger() const;

private:
    friend class CollisionsManager;

    void move(sf::Vector2f delta);
    void syncWithTransform();

    sf::FloatRect getBounds() const;

    void debugDraw(sf::RenderWindow& window);
};

#endif //COMPONENT_BASED_ARCH_COLLIDER_H
