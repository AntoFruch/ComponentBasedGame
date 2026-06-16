//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
#define COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
#include "Component.h"
#include "Managers/Scene/ComponentFactory.h"
#include <SFML/Graphics.hpp>

#include "Collider.h"


class CharacterController : public Component{
    Collider* collider;
public:
    CharacterController() = default;
    void Start() override;

    void move(const sf::Vector2f& delta);

private:
    static inline bool s_registered = ComponentFactory::Register("CharacterController", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        return std::make_unique<CharacterController>();
    });
};



#endif //COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
