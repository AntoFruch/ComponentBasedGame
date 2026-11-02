//
// Created by Antonin Fruchet on 28/10/2026.
//

#include "GameObjectFactory.h"

#include "ComponentFactory.h"

std::unique_ptr<GameObject> GameObjectFactory::buildGameObject(pugi::xml_node go)
{
    if (std::string nodeName = go.name(); nodeName != "GameObject")
    {
        throw IllegalOperationException("GameObject parser received a node that is not named \"GameObject\"");
    }

    auto ptr = std::make_unique<GameObject>(
                go.attribute("label").as_string(),
                sf::Vector2f{
                    go.attribute("x").as_float(),
                    go.attribute("y").as_float()
                },
                sf::degrees(go.attribute("angle").as_float()),
                sf::Vector2f{
                    go.attribute("sx") ? go.attribute("sx").as_float() : 1,
                    go.attribute("sy") ? go.attribute("sy").as_float() : 1,
                },
                go.attribute("active") ? go.attribute("active").as_bool() : true
                );

    auto components = go.child("Components");
    for (const auto& component : components.children())
    {
        if (std::string componentNodeName = component.name(); componentNodeName != "Component")
        {
            throw IllegalOperationException("Components node children must be named \"Component\"");
        }
        const std::string componentName = component.attribute("name").as_string();
        if (componentName.empty())
        {
            throw IllegalOperationException("Component node must have a non-empty \"name\" attribute");
        }
        ptr->addComponent(ComponentFactory::Create(componentName, component));
    }

    return std::move(ptr);
}
