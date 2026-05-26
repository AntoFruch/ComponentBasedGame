//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_CONTROLLER_H
#define COMPONENT_BASED_ARCH_CONTROLLER_H

#include "Component.h"
#include "InputManagement/InputAction.h"

class Controller : public Component {
    InputAction* moveAction;
    const float speed{100.f} ;
public:
    Controller();
    ~Controller() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
};



#endif //COMPONENT_BASED_ARCH_CONTROLLER_H
