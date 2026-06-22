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
    /**
     * @brief Called when the scene is fully created
     */
    virtual void Start();
    /**
     * @brief Called each frame
     * @param elapsedTime
     */
    virtual void Update(const sf::Time& elapsedTime);

    /**
     * @brief Changes the parent GameObject to go
     * @param go
     */
    void setParent(GameObject* go);
};



#endif //COMPONENT_BASED_ARCH_COMPONENT_H
