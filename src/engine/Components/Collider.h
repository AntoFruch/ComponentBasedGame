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
    sf::Vector2f dimensions;
    sf::Vector2f localPos;

    bool trigger;
    void (*callback)(const std::vector<Collider*>&);

public:
    Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger);
    ~Collider() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void move(sf::Vector2f delta);
    void syncWithTransform();

    void setPosition(sf::Vector2f);
    sf::FloatRect getBounds() const;

    void setTriggerCallback(void (*callback)(const std::vector<Collider*>&));

    [[nodiscard]] bool isTrigger() const;


    void debugDraw(sf::RenderWindow& window);
};



#endif //COMPONENT_BASED_ARCH_COLLIDER_H
