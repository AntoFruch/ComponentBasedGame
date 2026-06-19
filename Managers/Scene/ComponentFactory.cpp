//
// Created by Antonin Fruchet on 05/06/2026.
//

#include "ComponentFactory.h"
#include <format>
#include <unordered_map>

#include "exceptions/IllegalOperationException.h"

std::unique_ptr<Component> ComponentFactory::Create(const std::string& name, const pugi::xml_node& node)
{
    auto& registry = GetRegistry();
    if (auto it = registry.find(name); it != registry.end()) {
        return it->second(node); // Appelle la procédure de création à partir du node xml
    }
    throw IllegalOperationException(std::format("Class {} does not exist.", name));
}

bool ComponentFactory::Register(const std::string& name, Creator creator)
{
    auto& registry = GetRegistry();

    if (!registry.contains(name)) {
        registry[name] = creator;
        return true;
    }
    throw IllegalOperationException(std::format("Class {} is already register, two classes cannot have the same name", name));
}

std::unordered_map<std::string, ComponentFactory::Creator>& ComponentFactory::GetRegistry()
{
    static std::unordered_map<std::string, Creator> registry;
    return registry;
}
