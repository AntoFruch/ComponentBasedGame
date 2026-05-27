//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Renderer.h"

#include <iostream>

#include "Assets/GameObject.h"

Renderer::Renderer(const std::string& texture_path, const sf::Vector2u& spriteSize)
{
    loadTexture(texture_path);
    this->mShape.setSize(static_cast<sf::Vector2f>(spriteSize));
    this->spriteSize = spriteSize;
}
void Renderer::Start()
{
    cutRect.size = static_cast<sf::Vector2i>(spriteSize);
    setCutRectPos(0, 0);
    mShape.setTextureRect(cutRect);
}

void Renderer::loadTexture(const std::string& path)
{
    if (!texture.loadFromFile(path)) {
        std::cerr << "Texture at " << path << "not found" <<std::endl;
    }
    mShape.setTexture(&texture);
}

void Renderer::Update(const sf::Time& elapsedTime)
{
    mShape.setPosition(gameObject->transform.getPosition());
    mShape.setRotation(gameObject->transform.getRotation());
    mShape.setScale(gameObject->transform.getScale());
}

void Renderer::render(sf::RenderWindow& window) const
{
    window.draw(mShape);
}

void Renderer::setCutRectPos(unsigned int x, unsigned int y)
{
    cutRect.position = sf::Vector2i{static_cast<int>(x*spriteSize.x), static_cast<int>(y*spriteSize.y)};
    mShape.setTextureRect(cutRect);
}
