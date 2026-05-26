//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_RENDERER_H
#define COMPONENT_BASED_ARCH_RENDERER_H
#include <memory>

#include "Component.h"
#include "SFML/Graphics.hpp"


class Renderer : public Component {
    std::unique_ptr<sf::Shape> mShape;

public:
    Renderer();

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void render(sf::RenderWindow& window) const;
};



#endif //COMPONENT_BASED_ARCH_RENDERER_H
