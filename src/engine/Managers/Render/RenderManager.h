//
// Created by Antonin Fruchet on 04/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_RENDERMANAGER_H
#define COMPONENT_BASED_ARCH_RENDERMANAGER_H
#include "Components/Camera.h"
#include "Components/Renderer.h"


class RenderManager {
    static std::vector<Renderer*> mRenderers;
    static Camera* mainCamera;

public:
    static const float referenceHeight;

    static void registerRenderer(Renderer*);
    static void unregisterRenderer(const Renderer*);
    static void renderAll(sf::RenderWindow& window);

    static void setMainCamera(Camera*);

    static void handleResize(const sf::Vector2u& size);
};



#endif //COMPONENT_BASED_ARCH_RENDERMANAGER_H
