//
// Created by Antonin Fruchet on 05/06/2026.
//

#include "ComponentFactory.h"
#include <unordered_map>

std::unique_ptr<Component> ComponentFactory::Create(const std::string& name, const pugi::xml_node& node)
{
    auto& registry = GetRegistry();
    if (auto it = registry.find(name); it != registry.end()) {
        return it->second(node); // Appelle la procédure de création à partir du node xml
    }
    return nullptr;
}

void ComponentFactory::Register(const std::string& name, Creator creator)
{
    GetRegistry()[name] = creator;
}

std::unordered_map<std::string, ComponentFactory::Creator>& ComponentFactory::GetRegistry()
{
    static std::unordered_map<std::string, Creator> registry;
    return registry;
}
