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

    sf::Vector2f colliderPos;
    sf::Vector2f colliderSize;
public:
    CharacterController(const sf::Vector2f& pos, const sf::Vector2f& dimensions);
    void Start() override;

    void move(const sf::Vector2f& delta);

private:
    static inline bool s_registered = ComponentFactory::Register("CharacterController", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        return std::make_unique<CharacterController>(
            sf::Vector2f{
                node.attribute("colliderX").as_float(),
                node.attribute("colliderY").as_float()
            },
            sf::Vector2f{
                node.attribute("colliderW").as_float(),
                node.attribute("colliderH").as_float()
            }
        );
    });
};



#endif //COMPONENT_BASED_ARCH_CHARACTERCONTROLLER_H
