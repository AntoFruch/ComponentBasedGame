//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_CONTROLLER_H
#define COMPONENT_BASED_ARCH_CONTROLLER_H

#include "Component.h"
#include "AnimationHandling/Animator.h"
#include "InputManagement/InputAction.h"

class Controller : public Component {
    InputAction* moveAction;
    InputAction* slashAction;
    InputAction* wandAction;
    InputAction* bowAction;
    Animator* animator;

    const float speed{100.f} ;
public:
    Controller();
    ~Controller() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

private:
    void move(const sf::Time& elapsedTime);
    void slash(const sf::Time& elapsedTime);
    void wand(const sf::Time& elapsedTime);
    void bow(const sf::Time& elapsedTime);
};



#endif //COMPONENT_BASED_ARCH_CONTROLLER_H
