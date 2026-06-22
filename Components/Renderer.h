//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_RENDERER_H
#define COMPONENT_BASED_ARCH_RENDERER_H
#include <memory>

#include "Component.h"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/ComponentFactory.h"
#include "SFML/Graphics.hpp"


class Renderer : public Component {
    sf::RectangleShape mShape;
    sf::Texture texture;

    sf::Vector2u spriteSize;
    sf::IntRect cutRect;
    int layer;
    float sortOffsetY;

    void loadTexture(const std::string& path);
public:
    Renderer(
        const std::string& texture_path,
        const sf::Vector2f& anchor,
        const sf::Vector2u& spriteSize,
        int layer = 0,
        float sortOffsetY = 0.f
        );
    ~Renderer();

    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void render(sf::RenderWindow& window) const;

    void setCutRectPos(unsigned int x, unsigned int y);

    const sf::Vector2u getSpriteSize() const;
    int getLayer() const;
    float getSortY() const;

private:
    static inline bool s_registered = ComponentFactory::Register("Renderer", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        auto ptr = std::make_unique<Renderer>(
            node.attribute("src").as_string(),
            sf::Vector2f{
                node.attribute("anchorX").as_float(),
                node.attribute("anchorY").as_float()
            },
            sf::Vector2u{
                node.attribute("sprite_w").as_uint(),
                node.attribute("sprite_h").as_uint()
            },
            node.attribute("layer").as_int(0),
            node.attribute("sortOffsetY").as_float(0.f));
        RenderManager::registerRenderer(ptr.get());
        return ptr;
    });
};



#endif //COMPONENT_BASED_ARCH_RENDERER_H
