//
// Created by Antonin Fruchet on 05/06/2026.
//

#include "Scene.h"
#include "SceneManager.h"
#include <iostream>

#include "ComponentFactory.h"
#include "exceptions/IllegalOperationException.h"

void Scene::Start()
{
    for (auto& go : mTargets)
    {
        go->start();
    }
}

void Scene::Update(const sf::Time& elapsedTime)
{
    for (auto& go : mTargets)
    {
        go->update(elapsedTime);
    }
    std::erase_if(mTargets, [](const std::unique_ptr<GameObject>& go)
    {
        return go->isWaitingDestruction();
    });
    if (SceneManager::loadingReq) SceneManager::applyRequest();
    if (instantiateRequested) applyInstantiate();
}

std::unique_ptr<Component> Scene::build_component(const pugi::xml_node& c) // Plus de GameObject* ici
{
    std::string name = c.attribute("name").as_string();
    return ComponentFactory::Create(name, c);

}

std::unique_ptr<GameObject> Scene::build_go(const pugi::xml_node& go, Transform* parent)
{
    auto ptr = std::make_unique<GameObject>(
                go.attribute("label").as_string(),
                sf::Vector2f{go.attribute("x").as_float(),go.attribute("y").as_float()},
                sf::degrees(go.attribute("angle").as_float()),
                sf::Vector2f{go.attribute("sx").as_float(),go.attribute("sy").as_float()},
                go.attribute("active") ? go.attribute("active").as_bool() : true
                );
    ptr->transform.setParent(parent);

    for (const auto& c : go.children())
    {
        std::string name = c.name();
        if (name=="Components")
        {
            for (const auto& component : c.children("Component"))
            {
                ptr->addComponent(build_component(component));
            }
        }
        if (name == "GameObject")
        {
            ptr->addChild(build_go(c, &ptr->transform));
        }
        if (name == "Prefab")
        {
            ptr->addChild(build_prefab(c, &ptr->transform));
        }
    }
    return std::move(ptr);
}
std::unique_ptr<GameObject> Scene::build_prefab(const pugi::xml_node& obj, Transform* parent)
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

    auto prefab = build_go(root_node, parent);
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

void Scene::load(std::string_view scenePath)
{
    std::filesystem::path path = scenePath;
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
            mTargets.push_back(build_go(obj, nullptr));
            continue;
        }
        if ( name == "Prefab")
        {
            mTargets.push_back(build_prefab(obj, nullptr));
            continue;
        }
        throw IllegalOperationException("Node Scene children must be GameObject or Prefab");
    }
}

void Scene::unload()
{
    mTargets.clear();
}

void Scene::applyInstantiate()
{
    instantiateRequested = false;
    requestGO->start();
    mTargets.push_back(std::move(requestGO));
}

GameObject* Scene::requestInstantiate(std::string_view prefabPath)
{
    std::filesystem::path path = prefabPath;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result) {
        std::cerr << "Could not open prefab because " << result.description() << std::endl;
    }
    auto root_node = doc.child("GameObject");
    if (!root_node)
    {
        throw IllegalOperationException("Prefab files must have a unique root node named \"GameObject\"");
    }

    auto prefab = build_go(root_node, nullptr);
    auto ret_ptr = prefab.get();
    requestGO = std::move(prefab);

    instantiateRequested = true;

    return ret_ptr;
}
