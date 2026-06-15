//
// Created by Antonin Fruchet on 05/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COMPONENTFACTORY_H
#define COMPONENT_BASED_ARCH_COMPONENTFACTORY_H
#include <functional>
#include <unordered_map>

#include "pugixml.hpp"
#include "Components/Component.h"

class ComponentFactory {
public:
    using Creator = std::function<std::unique_ptr<Component>(const pugi::xml_node& node)>;

    static bool Register(const std::string& name, Creator creator);

    static std::unique_ptr<Component> Create(const std::string& name, const pugi::xml_node& node);

private:
    static std::unordered_map<std::string, Creator>& GetRegistry();
};

#endif //COMPONENT_BASED_ARCH_COMPONENTFACTORY_H
