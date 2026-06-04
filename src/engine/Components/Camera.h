//
// Created by Antonin Fruchet on 04/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_CAMERA_H
#define COMPONENT_BASED_ARCH_CAMERA_H
#include "Component.h"
#include "SFML/Graphics.hpp"


class Camera : public Component {
    sf::View m_view;

public:
    Camera()=default;

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void applyView(sf::RenderWindow& window) const;

    void updateSize(sf::Vector2u size);
};



#endif //COMPONENT_BASED_ARCH_CAMERA_H
