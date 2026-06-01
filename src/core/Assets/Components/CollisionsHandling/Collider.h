//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLIDER_H
#define COMPONENT_BASED_ARCH_COLLIDER_H
#include "../Component.h"
#include "SFML/Graphics.hpp"

class
Collider : public Component {
    sf::RectangleShape hitbox;
    sf::Vector2f dimensions;
    sf::Vector2f localPos;

    bool trigger;

public:
    Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger);
    ~Collider() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void move(sf::Vector2f delta);
    void syncWithTransform();

    void setPosition(sf::Vector2f);
    sf::FloatRect getBounds() const;

    void debugDraw(sf::RenderWindow& window);

    [[nodiscard]] bool isTrigger() const;
};



#endif //COMPONENT_BASED_ARCH_COLLIDER_H
