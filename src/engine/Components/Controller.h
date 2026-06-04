//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_CONTROLLER_H
#define COMPONENT_BASED_ARCH_CONTROLLER_H

#include "CharacterController.h"
#include "Animator.h"
#include "Collider.h"
#include "Managers/Input/InputAction.h"

class Controller : public CharacterController {
    InputAction* moveAction;
    InputAction* slashAction;
    Animator* animator;

    const float speed{100.f} ;
    sf::Vector2f facing{0.f, -1.f};
public:
    Controller(const sf::Vector2f& pos, const sf::Vector2f& dimensions);
    ~Controller() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

private:
    void handleMovement(const sf::Time& elapsedTime);

    void slash(const sf::Time& elapsedTime);
};



#endif //COMPONENT_BASED_ARCH_CONTROLLER_H
