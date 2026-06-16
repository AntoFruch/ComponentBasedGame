//
// Created by Antonin Fruchet on 04/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_CAMERA_H
#define COMPONENT_BASED_ARCH_CAMERA_H
#include "Component.h"
#include "Managers/Scene/ComponentFactory.h"
#include "SFML/Graphics.hpp"

std::unique_ptr<Component> create_camera(pugi::xml_node const& node);

class Camera : public Component {
    sf::View m_view;

public:
    Camera()=default;

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void applyView(sf::RenderWindow& window) const;

    void updateSize(sf::Vector2u size);

private:
    static inline bool s_registered = ComponentFactory::Register("Camera", create_camera);
};

inline std::unique_ptr<Component> create_camera(pugi::xml_node const& node)
{
    return std::make_unique<Camera>();
}



#endif //COMPONENT_BASED_ARCH_CAMERA_H
