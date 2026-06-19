//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "RenderManager.h"

#include <set>

#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Scene/SceneManager.h"

std::vector<Renderer*> RenderManager::mRenderers{};
Camera* RenderManager::mainCamera{nullptr};
sf::RenderWindow* RenderManager::window{nullptr};

const float RenderManager::referenceHeight = 480;

void RenderManager::init(sf::RenderWindow* win)
{
    window = win;
    tgui::Texture::setDefaultSmooth(false);
}

void RenderManager::registerRenderer(Renderer* renderer)
{
    mRenderers.push_back(renderer);
}

void RenderManager::unregisterRenderer(const Renderer* renderer)
{
    std::erase_if(mRenderers,[&renderer](const Renderer* r){ return r==renderer;} );
}

void RenderManager::renderAll(sf::RenderWindow& window)
{
    window.clear();

    // best sorting algo for almost sorted lists;
    std::ranges::stable_sort(mRenderers, std::less{}, [](const Renderer* r) {
        return r->gameObject->transform.getWorldPosition().y - r->getSpriteSize().y;
    });

    if (mainCamera){
        mainCamera->applyView(window);
    }
    for (auto& renderer : mRenderers)
    {
        renderer->render(window);
    }

    SceneManager::getGui()->draw();
}

void RenderManager::setMainCamera(Camera* cam)
{
    mainCamera = cam;
    handleResize(window->getSize());
}

const sf::RenderWindow* RenderManager::getWindow()
{
    return window;
}

void RenderManager::handleResize(const sf::Vector2u& size) {
    if (mainCamera) {
        mainCamera->updateSize(size);
    }
}
