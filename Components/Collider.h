//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLIDER_H
#define COMPONENT_BASED_ARCH_COLLIDER_H
#include "Component.h"
#include "Managers/Scene/ComponentFactory.h"
#include "SFML/Graphics.hpp"
#include <array>
#include <functional>

class Collider;
using TriggerCallback =  std::function<void(const std::vector<Collider*>&, Collider*)>;

class Collider : public Component {
    sf::RectangleShape hitbox;
    sf::Vector2f localPos;

    /**
     * true : type trigger ( no solid collisions)
     * false: type solid ( solid collisions)
     */
    bool trigger;

    /**
     * @brief function ptr, the function is executed when a trigger collider triggers
     * must have arguments :
     * @param hits vector of pointers of hit targets
     * @param self pointer to the trigger
     */
    TriggerCallback callback;

public:
    Collider(const sf::Vector2f& pos, const sf::Vector2f& dimensions, bool trigger);
    ~Collider() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;

    void setPosition(sf::Vector2f);
    void setSize(sf::Vector2f);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();

    [[nodiscard]] bool isTrigger() const;

    void setTriggerCallback(std::function<void(const std::vector<Collider*>&, Collider*)> callback);

private:
    friend class CollisionsManager;

    void move(sf::Vector2f delta);
    void syncWithTransform();

    sf::FloatRect getBounds() const;
    std::array<sf::Vector2f, 4> getWorldCorners() const;
    bool intersects(const Collider& other) const;

    void debugDraw(sf::RenderWindow& window);

    static inline bool s_registered = ComponentFactory::Register("Collider", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        return std::make_unique<Collider>(
            sf::Vector2f{
                node.attribute("x").as_float(),
                node.attribute("y").as_float(),
            },
            sf::Vector2f{
                node.attribute("width").as_float(),
                node.attribute("height").as_float(),
            },
            node.attribute("trigger").as_bool()
        );
    });
};

#endif //COMPONENT_BASED_ARCH_COLLIDER_H
