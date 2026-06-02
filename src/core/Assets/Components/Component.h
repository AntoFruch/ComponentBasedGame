//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_COMPONENT_H
#define COMPONENT_BASED_ARCH_COMPONENT_H
#include "SFML/System/Time.hpp"

class GameObject;


class Component {
public:
    explicit Component();
    virtual ~Component()=default;

    GameObject* gameObject;
    
    virtual void Start();
    virtual void Update(const sf::Time& elapsedTime);
    void setParent(GameObject* go);

protected:
};



#endif //COMPONENT_BASED_ARCH_COMPONENT_H
