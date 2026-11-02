//
// Created by Antonin Fruchet on 28/10/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMEOBJECTFACTORY_H
#define COMPONENT_BASED_ARCH_GAMEOBJECTFACTORY_H

#include "GameObject.h"
#include "pugixml.hpp"

class GameObjectFactory {
public:
    static std::unique_ptr<GameObject> buildGameObject(pugi::xml_node);
};



#endif //COMPONENT_BASED_ARCH_GAMEOBJECTFACTORY_H
