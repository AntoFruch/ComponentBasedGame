//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEMANAGER_H
#define COMPONENT_BASED_ARCH_SCENEMANAGER_H

#include "pugixml.hpp"
#include "Scene.h"
#include "Components/Renderer.h"

class SceneManager {
    static Scene* scene;

    static bool loadingReq;
    static std::string reqPath;
public:
    static void init(Scene* ptr);
    static void Update(const sf::Time& elapsedTime);
    /**
     * @brief Enregistre une demande de changement de scène, le changement sera effectif à la fin de la boucle d'update
     * @param path
     */
    static void requestLoading(std::string_view path);

    static GameObject* instantiate(std::string_view prefab);

private:
    friend class Scene;
    static void applyRequest();
};



#endif //COMPONENT_BASED_ARCH_SCENEMANAGER_H
