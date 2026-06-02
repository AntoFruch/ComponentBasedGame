//
// Created by Antonin Fruchet on 02/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_SLIMECONTROLLER_H
#define COMPONENT_BASED_ARCH_SLIMECONTROLLER_H
#include "Component.h"
#include "AnimationHandling/Animator.h"


class SlimeController : public Component {
    Animator* animator;

public:
    SlimeController();

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void takeDamage();
};



#endif //COMPONENT_BASED_ARCH_SLIMECONTROLLER_H
