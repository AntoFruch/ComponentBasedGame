//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_TRANSFORM_H
#define COMPONENT_BASED_ARCH_TRANSFORM_H
#include <vector>

#include "SFML/Graphics.hpp"


class Transform {
    sf::Vector2f position;
    sf::Angle rotation; // deg
    sf::Vector2f scale;

    Transform* parent{nullptr};
    std::vector<Transform*> children;

public:
    explicit Transform() = default;
    explicit Transform(const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, Transform* parent);

    const std::vector<Transform*>& getChildren() const;
    void addChild(Transform* child);
    void setParent(Transform* parent);

    const sf::Vector2f& getLocalPosition() const;
    const sf::Angle& getLocalRotation() const;
    const sf::Vector2f& getLocalScale() const;
    sf::Vector2f getWorldPosition() const;
    sf::Angle getWorldRotation() const;
    sf::Vector2f getWorldScale() const;

    void set_position(const sf::Vector2f& position);
    void set_rotation(const sf::Angle& rotation);
    void set_scale(const sf::Vector2f& scale);
    
    void move(sf::Vector2f amount);
    void rotate(sf::Angle amount);
    void rescale(sf::Vector2f amount);
};



#endif //COMPONENT_BASED_ARCH_TRANSFORM_H
