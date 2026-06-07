//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Renderer.h"

#include <iostream>

#include "GameObject.h"
#include "Collider.h"
#include "pugixml.hpp"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/ComponentFactory.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("Renderer", [](const pugi::xml_node& node) {
            auto ptr = std::make_unique<Renderer>(
                node.attribute("src").as_string(),
                sf::Vector2f{
                    node.attribute("anchorX").as_float(),
                    node.attribute("anchorY").as_float()
                },
                sf::Vector2u{
                    node.attribute("sprite_w").as_uint(),
                    node.attribute("sprite_h").as_uint()
                });
            RenderManager::registerRenderer(ptr.get());
            return std::move(ptr);
        });
        return true;
    }();
}
// --------------------------



Renderer::Renderer(const std::string& texture_path, const sf::Vector2f& anchor,  const sf::Vector2u& spriteSize)
{
    loadTexture(texture_path);
    const auto size = static_cast<sf::Vector2f>(spriteSize);
    this->mShape.setSize(size);
    this->mShape.setOrigin(anchor + size / 2.f);
    this->spriteSize = spriteSize;
}

Renderer::~Renderer()
{
    RenderManager::unregisterRenderer(this);
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
    mShape.setPosition(gameObject->transform.getWorldPosition());
    mShape.setRotation(gameObject->transform.getWorldRotation());
    mShape.setScale(gameObject->transform.getWorldScale());
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
