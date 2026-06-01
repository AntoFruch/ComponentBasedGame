//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_CONTROLLER_H
#define COMPONENT_BASED_ARCH_CONTROLLER_H

#include "CharacterController.h"
#include "AnimationHandling/Animator.h"
#include "CollisionsHandling/Collider.h"
#include "InputManagement/InputAction.h"

class Controller : public CharacterController {
    InputAction* moveAction;
    InputAction* slashAction;
    InputAction* wandAction;
    InputAction* bowAction;
    InputAction* hitAction;
    Animator* animator;

    const float speed{100.f} ;
public:
    Controller(const sf::Vector2f& pos, const sf::Vector2f& dimensions);
    ~Controller() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

private:
    void handleMovement(const sf::Time& elapsedTime);

    void slash(const sf::Time& elapsedTime);
    void wand(const sf::Time& elapsedTime);
    void bow(const sf::Time& elapsedTime);
    void hit(const sf::Time& elapsedTime);
};



#endif //COMPONENT_BASED_ARCH_CONTROLLER_H
