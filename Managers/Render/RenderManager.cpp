//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "RenderManager.h"

#include <set>

#include "Managers/Collisions/CollisionsManager.h"

std::vector<Renderer*> RenderManager::mRenderers{};
Camera* RenderManager::mainCamera{nullptr};
sf::RenderWindow* RenderManager::window{nullptr};

const float RenderManager::referenceHeight = 480;

void RenderManager::init(sf::RenderWindow* win)
{
    window = win;
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

    if (mainCamera){
        mainCamera->applyView(window);
    }
    for (auto& renderer : mRenderers)
    {
        renderer->render(window);
    }
}

void RenderManager::setMainCamera(Camera* cam)
{
    mainCamera = cam;
    handleResize(window->getSize());
}

void RenderManager::handleResize(const sf::Vector2u& size) {
    if (mainCamera) {
        mainCamera->updateSize(size);
    }
}