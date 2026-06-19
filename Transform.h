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

    static sf::Vector2f rotateVector(sf::Vector2f v, sf::Angle angle);

    const std::vector<Transform*>& getChildren() const;
    void addChild(Transform* child);
    void removeChild(Transform* child);
    void setParent(Transform* parent);
    [[nodiscard]] Transform* getParent() const;

    const sf::Vector2f& getLocalPosition() const;
    const sf::Angle& getLocalRotation() const;
    const sf::Vector2f& getLocalScale() const;
    sf::Vector2f getWorldPosition() const;
    sf::Angle getWorldRotation() const;
    sf::Vector2f getWorldScale() const;

    void setLocalPosition(const sf::Vector2f& position);
    void setLocalRotation(const sf::Angle& rotation);
    void setLocalScale(const sf::Vector2f& scale);
    
    void move(sf::Vector2f amount);
    void rotate(sf::Angle amount);
    void rescale(sf::Vector2f amount);
};



#endif //COMPONENT_BASED_ARCH_TRANSFORM_H
