//
// Created by Antonin Fruchet on 04/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_CAMERA_H
#define COMPONENT_BASED_ARCH_CAMERA_H
#include "Component.h"
#include "Managers/Scene/ComponentFactory.h"
#include "SFML/Graphics.hpp"


class Camera : public Component {
    sf::View m_view;

public:
    Camera()=default;
    ~Camera();

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void applyView(sf::RenderWindow& window) const;

    void updateSize(sf::Vector2u size);

private:
    static inline bool s_registered = ComponentFactory::Register("Camera", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        return std::make_unique<Camera>();
    });
};



#endif //COMPONENT_BASED_ARCH_CAMERA_H
