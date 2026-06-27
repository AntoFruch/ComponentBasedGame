//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "Camera.h"
#include "GameObject.h"

#include "Managers/Render/RenderManager.h"

Camera::~Camera()
{
    RenderManager::unsetMainCamera(this);
}

void Camera::Start()
{
    Component::Start();
    RenderManager::setMainCamera(this);
}

void Camera::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    m_view.setCenter(gameObject->transform.getWorldPosition());
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
