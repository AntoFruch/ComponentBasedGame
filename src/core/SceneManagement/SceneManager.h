//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEMANAGER_H
#define COMPONENT_BASED_ARCH_SCENEMANAGER_H
#include <vector>

#include "Assets/GameObject.h"


class SceneManager {
    inline static const std::filesystem::path scenes_dir{"resources/scenes/"};

public :
    static void loadScene(std::string_view scene, std::vector<std::unique_ptr<GameObject>>& targets);
    static void unloadScene(std::vector<std::unique_ptr<GameObject>>& targets);
};



#endif //COMPONENT_BASED_ARCH_SCENEMANAGER_H
