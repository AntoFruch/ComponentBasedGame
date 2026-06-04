//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEMANAGER_H
#define COMPONENT_BASED_ARCH_SCENEMANAGER_H
#include <vector>

#include "pugixml.hpp"
#include "GameObject.h"
#include "Components/Renderer.h"


class SceneManager {
    inline static const std::filesystem::path scenes_dir{"resources/scenes/"};

    static std::unique_ptr<Component> build_component(const pugi::xml_node& c);
    static std::unique_ptr<GameObject> build_go(const pugi::xml_node& go, Transform* parent);
    static std::unique_ptr<GameObject> build_prefab(const pugi::xml_node& obj, Transform* parent);
public :
    static void loadScene(std::string_view scene, std::vector<std::unique_ptr<GameObject>>& targets);
    static void unloadScene(std::vector<std::unique_ptr<GameObject>>& targets);
};



#endif //COMPONENT_BASED_ARCH_SCENEMANAGER_H
