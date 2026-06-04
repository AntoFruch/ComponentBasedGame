//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
#define COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
#include "Component.h"
#include <SFML/Graphics.hpp>

#include "Collider.h"


class CharacterController : public Component{
    Collider* collider;

    sf::Vector2f colliderPos;
    sf::Vector2f colliderSize;
public:
    CharacterController(const sf::Vector2f& pos, const sf::Vector2f& dimensions);
    void Start() override;

    void move(const sf::Vector2f& delta);
};



#endif //COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
