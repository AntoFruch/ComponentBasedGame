//
// Created by Antonin Fruchet on 05/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENE_H
#define COMPONENT_BASED_ARCH_SCENE_H

#include <pugixml.hpp>

#include "GameObject.h"

class Scene {
    std::vector<std::unique_ptr<GameObject>> mObjects;
    

    bool instantiateRequested{false};

    /**
     * The new instantiated GameObject (requestIntantiate() method) that will be inserted in mTargets
     */
    std::unique_ptr<GameObject> requestGO;

public:
    Scene() = default;
    void Start();
    void Update(const sf::Time& elapsedTime);

    void load(std::string_view scene);
    void unload();

    void applyInstantiate();
    GameObject* requestInstantiate(std::string_view prefabPath);

    std::string dump() const;

private:
    std::unique_ptr<GameObject> build_go(const pugi::xml_node& go);
    std::unique_ptr<GameObject> build_prefab(const pugi::xml_node& obj);

};



#endif //COMPONENT_BASED_ARCH_SCENE_H
