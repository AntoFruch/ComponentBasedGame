//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "Camera.h"

#include <iostream>

#include "Managers/RenderManager.h"

void Camera::Start()
{
    Component::Start();
}

void Camera::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    m_view.setCenter(gameObject->transform.getWorldPosition());
    std::cout<< "cam set at : " << gameObject->transform.getWorldPosition().x << " " << gameObject->transform.getWorldPosition().y << std::endl;
}

void Camera::updateSize(sf::Vector2u size)
{
    float windowRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
    auto newWidth = windowRatio * RenderManager::referenceHeight;
    m_view.setSize(sf::Vector2f{newWidth, RenderManager::referenceHeight});
}

void Camera::applyView(sf::RenderWindow& window) const
{
    window.setView(m_view);
}
