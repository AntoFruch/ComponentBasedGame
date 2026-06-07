//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_RENDERER_H
#define COMPONENT_BASED_ARCH_RENDERER_H
#include <memory>

#include "Component.h"
#include "SFML/Graphics.hpp"


class Renderer : public Component {
    sf::RectangleShape mShape;
    sf::Texture texture;

    sf::Vector2u spriteSize;
    sf::IntRect cutRect;

    void loadTexture(const std::string& path);
public:
    Renderer(const std::string& texture_path, const sf::Vector2f& anchor, const sf::Vector2u& spriteSize);
    ~Renderer();

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void render(sf::RenderWindow& window) const;

    void setCutRectPos(unsigned int x, unsigned int y);
};



#endif //COMPONENT_BASED_ARCH_RENDERER_H
