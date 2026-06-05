//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "Camera.h"
#include "GameObject.h"

#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/ComponentFactory.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("Camera", [](const pugi::xml_node& node) {
            return std::make_unique<Camera>();
        });
        return true;
    }();
}
// --------------------------

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
