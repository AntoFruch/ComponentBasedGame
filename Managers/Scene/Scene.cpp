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
    for (auto& go : mObjects)
    {
        go->start();
    }
}

void Scene::Update(const sf::Time& elapsedTime)
{
    for (auto& go : mObjects)
    {
        go->update(elapsedTime);
    }
    std::erase_if(mObjects, [](const std::unique_ptr<GameObject>& go)
    {
        return go->isWaitingDestruction();
    });
    if (SceneManager::loadingReq) SceneManager::applyRequest();
    if (instantiateRequested) applyInstantiate();
}

std::unique_ptr<GameObject> Scene::build_go(const pugi::xml_node& go)
{
    auto ptr = std::make_unique<GameObject>(
                go.attribute("label").as_string(),
                sf::Vector2f{go.attribute("x").as_float(),go.attribute("y").as_float()},
                sf::degrees(go.attribute("angle").as_float()),
                sf::Vector2f{go.attribute("sx").as_float(),go.attribute("sy").as_float()},
                go.attribute("active") ? go.attribute("active").as_bool() : true
                );

    for (const auto& c : go.children())
    {
        std::string name = c.name();
        if (name=="Components")
        {
            for (const auto& component : c.children("Component"))
            {
                ptr->addComponent(ComponentFactory::Create(component.attribute("name").as_string(), component));
            }
        }
        if (name == "GameObject")
        {
            ptr->addChild(build_go(c));
        }
        if (name == "Prefab")
        {
            ptr->addChild(build_prefab(c));
        }
    }
    return std::move(ptr);
}
std::unique_ptr<GameObject> Scene::build_prefab(const pugi::xml_node& obj)
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

    auto prefab = build_go(root_node);
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
            mObjects.push_back(build_go(obj));
            continue;
        }
        if ( name == "Prefab")
        {
            mObjects.push_back(build_prefab(obj));
            continue;
        }
        throw IllegalOperationException("Node Scene children must be GameObject or Prefab");
    }
}

void Scene::unload()
{
    mObjects.clear();
}

void Scene::applyInstantiate()
{
    instantiateRequested = false;
    requestGO->start();
    mObjects.push_back(std::move(requestGO));
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

std::string Scene::dump() const
{
    std::ostringstream oss;
    oss << "Scene :\n";

    // Lambda récursive pour parcourir la hiérarchie
    // On passe l'indentation sous forme de chaîne de caractères (ex: "  ")
    auto dumpGameObject = [&oss](const auto& self, const auto& go, const std::string& indent) -> void {
        if (!go) return;

        // 1. On affiche le nom du GameObject actuel avec son indentation
        oss << indent << "- " << go->getLabel() << "\n";

        // 2. On parcourt récursivement ses enfants en augmentant l'indentation
        for (const auto& child : go->getChildren())
        {
            self(self, child, indent + "  "); // Ajoute 2 espaces par niveau
        }
    };

    // Lancement du parcours pour chaque GameObject racine de la scène
    for (const auto& go : mObjects)
    {
        dumpGameObject(dumpGameObject, go, "  ");
    }

    return oss.str();
}
