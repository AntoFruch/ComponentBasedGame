//
// Created by Antonin Fruchet on 05/06/2026.
//

#include "Scene.h"
#include "SceneManager.h"
#include <format>
#include <iostream>

#include "GameObjectFactory.h"
#include "exceptions/IllegalOperationException.h"

Scene::Scene(sf::RenderWindow& window) : mGui(window)
{
}

void Scene::Start()
{
    for (auto& go : mObjects)
    {
        go->start();
    }
}

void Scene::Update(const sf::Time& elapsedTime)
{
    if (!frozen) {
        for (auto& go : mObjects)
        {
            go->update(elapsedTime);
        }
        std::erase_if(mObjects, [](const std::unique_ptr<GameObject>& go)
        {
            return go->isWaitingDestruction();
        });
    }

    if (SceneManager::loadingReq) SceneManager::applyRequest();
    if (!requestedGOs.empty()) applyInstantiate();
}

pugi::xml_node Scene::open_prefab(const pugi::xml_node& obj)
{
    const std::string prefabPath = obj.attribute("src").as_string();
    if (prefabPath.empty())
    {
        throw IllegalOperationException("Prefab node must have a non-empty \"src\" attribute");
    }

    auto doc = std::make_unique<pugi::xml_document>();
    if (auto result = doc->load_file(prefabPath.c_str()); !result) {
        throw IllegalOperationException(std::format("Could not open prefab because {}", result.description()));
    }
    auto root_node = doc->child("GameObject");
    if (!root_node)
    {
        throw IllegalOperationException("Prefab files must have a unique root node named \"GameObject\"");
    }

    mPrefabDocs.push_back(std::move(doc));
    return root_node;
}

void Scene::load(std::string_view scenePath)
{
    std::filesystem::path path = scenePath;
    pugi::xml_document doc;
    std::cout << scenePath << std::endl;
    if (auto result = doc.load_file(path.c_str()); !result) {
        throw IllegalOperationException(std::format("Could not open scene because {}", result.description()));
    }

    pugi::xml_node sceneObj = doc.child("Scene");
    if (!sceneObj)
    {
        throw IllegalOperationException("Root node of scene must be named \"Scene\"");
    }
    pugi::xml_node cur;
    std::queue<pugi::xml_node> queue{{sceneObj}};
    while (!queue.empty())
    {
        cur = queue.front();
        queue.pop();

        if (cur.child("GameObject").empty() && cur.child("Prefab").empty())
            continue;

        for (const auto& obj : cur)
        {
            std::string name = obj.name();
            if ( name == "GameObject")
            {
                mObjects.push_back(GameObjectFactory::buildGameObject(obj));
                queue.push(obj);
                continue;
            }
            if ( name == "Prefab")
            {
                auto new_obj = open_prefab(obj);
                std::cout << new_obj.name() << std::endl;
                auto prefab = GameObjectFactory::buildGameObject(new_obj);
                if (obj.attribute("label"))
                {
                    prefab->setLabel(obj.attribute("label").as_string());
                }
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
                mObjects.push_back(std::move(prefab));
                queue.push(new_obj);
                continue;
            }
            throw IllegalOperationException("Node Scene children must be GameObject, Prefab or UI");
        }
    }
}

void Scene::unload()
{
    mObjects.clear();
}

void Scene::applyInstantiate()
{
    for (auto& requestedGO : requestedGOs)
    {
        requestedGO->start();
        mObjects.push_back(std::move(requestedGO));
    }
    requestedGOs.clear();
}

GameObject* Scene::requestInstantiate(std::string_view prefabPath)
{
    std::filesystem::path path = prefabPath;
    if (path.empty())
    {
        throw IllegalOperationException("Prefab path must be non-empty");
    }
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result) {
        throw IllegalOperationException(std::format("Could not open prefab because {}", result.description()));
    }
    auto root_node = doc.child("GameObject");
    if (!root_node)
    {
        throw IllegalOperationException("Prefab files must have a unique root node named \"GameObject\"");
    }

    auto prefab = GameObjectFactory::buildGameObject(root_node);
    auto ret_ptr = prefab.get();
    requestedGOs.push_back(std::move(prefab));

    return ret_ptr;
}

void Scene::freeze()
{
    frozen=true;
}

void Scene::unfreeze()
{
    frozen=false;
}

tgui::Gui* Scene::getGui()
{
    return &mGui;
}

std::string Scene::dump() const
{
    /* FIXME: Dump not funtional anymore
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
    */
    return "dump not functional anymore";
}
