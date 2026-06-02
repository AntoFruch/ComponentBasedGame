//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "SceneManager.h"
#include <iostream>
#include <__ranges/rend.h>

#include "pugixml.hpp"
#include "../Assets/Components/CollisionsHandling/Collider.h"
#include "Assets/Components/Controller.h"
#include "Assets/Components/SlimeController.h"
#include "Assets/Components/AnimationHandling/Animator.h"
#include "exceptions/IllegalOperationException.h"

std::unique_ptr<Component> SceneManager::build_component(const pugi::xml_node& c, std::vector<Renderer*>& renderers) // Plus de GameObject* ici
{
    std::string name = c.attribute("name").as_string();
    if (name == "Renderer")
    {
        auto ptr = std::make_unique<Renderer>(
             c.attribute("src").as_string(),
                        sf::Vector2u{
                            c.attribute("sprite_w").as_uint(),
                            c.attribute("sprite_h").as_uint()
                        });
        renderers.push_back(ptr.get());
        return std::move(ptr); // Constructeur par défaut ou sans le GO
    }
    if (name == "Controller")
    {
        return std::make_unique<Controller>(sf::Vector2f{c.attribute("x").as_float(), c.attribute("y").as_float()},
            sf::Vector2f{c.attribute("width").as_float(), c.attribute("height").as_float()});
    }
    if (name == "SlimeController")
    {
        return std::make_unique<SlimeController>();
    }
    if (name == "Animator")
    {
        return std::make_unique<Animator>(c.attribute("src").as_string());
    }
    if (name == "Collider")
    {
        return std::make_unique<Collider>(
            sf::Vector2f{c.attribute("x").as_float(), c.attribute("y").as_float()},
            sf::Vector2f{c.attribute("width").as_float(), c.attribute("height").as_float()},
            c.attribute("trigger").as_bool());
    }
    return nullptr;
}

std::unique_ptr<GameObject> SceneManager::build_go(const pugi::xml_node& go, Transform* parent,  std::vector<Renderer*>& renderers)
{
    auto ptr = std::make_unique<GameObject>(
        // ajouter label si besoin, mais pas obligé je pense
                go.attribute("label").as_string(),
                sf::Vector2f{go.attribute("x").as_float(),go.attribute("y").as_float()},
                sf::degrees(go.attribute("angle").as_float()),
                sf::Vector2f{go.attribute("sx").as_float(),go.attribute("sy").as_float()},
                go.attribute("active").as_bool()
                );
    ptr->transform.setParent(parent);

    for (const auto& c : go.children())
    {
        std::string name = c.name();
        if (name=="Components")
        {
            for (const auto& component : c.children("Component"))
            {
                ptr->addComponent(build_component(component, renderers));
            }
        }
        if (name == "GameObject")
        {
            ptr->addChild(build_go(c, &ptr->transform, renderers));
        }
        if (name == "Prefab")
        {
            ptr->addChild(build_prefab(c, &ptr->transform, renderers));
        }
    }
    return std::move(ptr);
}
std::unique_ptr<GameObject> SceneManager::build_prefab(const pugi::xml_node& obj, Transform* parent, std::vector<Renderer*>& renderers)
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(obj.attribute("src").as_string()); !result) {
        std::cerr << "Could not open prefab because " << result.description() << std::endl;
    }
    auto root_node = doc.child("GameObject");
    if (!root_node)
    {
        throw IllegalOperationException("Prefab files must have a unique root node named \"GameObject\"");
    }

    auto prefab = build_go(root_node, parent, renderers);
    if (obj.attribute("x") || obj.attribute("y"))
    {
        prefab->transform.move({obj.attribute("x").as_float(), obj.attribute("y").as_float()});
    }
    if (obj.attribute("angle"))
    {
        prefab->transform.rotate(sf::degrees(obj.attribute("angle").as_float()));
    }
    if (obj.attribute("sx") || obj.attribute("sy"))
    {
        prefab->transform.rescale({obj.attribute("sx").as_float(1.f), obj.attribute("sy").as_float(1.f)});
    }
    if (obj.attribute("active"))
    {
        prefab->setActive(obj.attribute("active").as_bool());
    }
    return prefab;
}

void SceneManager::loadScene(std::string_view scene, std::vector<std::unique_ptr<GameObject>>& targets, std::vector<Renderer*>& renderers)
{
    std::filesystem::path path = scenes_dir / scene;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result) {
        std::cerr << "Could not open scene because " << result.description() << std::endl;
    }

    pugi::xml_node sceneObj = doc.child("Scene");
    for (const auto& obj : sceneObj)
    {
        std::string name = obj.name();
        if ( name == "GameObject")
        {
            targets.push_back(build_go(obj, nullptr, renderers));
            continue;
        }
        if ( name == "Prefab")
        {
            targets.push_back(build_prefab(obj, nullptr, renderers));
            continue;
        }
        throw IllegalOperationException("Scene node children must be GameObject or Prefab");
    }
}

void SceneManager::unloadScene(std::vector<std::unique_ptr<GameObject>>& targets)
{
    targets.clear();
}
