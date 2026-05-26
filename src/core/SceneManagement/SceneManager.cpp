//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "SceneManager.h"
#include <iostream>
#include "pugixml.hpp"
#include "Assets/Components/Controller.h"

std::unique_ptr<Component> build_component(const pugi::xml_node& c) // Plus de GameObject* ici
{
    std::string name = c.attribute("name").as_string();
    if (name == "Controller")
    {
        return std::make_unique<Controller>(); // Constructeur par défaut ou sans le GO
    }
    return nullptr;
}

std::unique_ptr<GameObject> build_go(pugi::xml_node go)
{
    auto ptr = std::make_unique<GameObject>(
        // ajouter label si besoin, mais pas obligé je pense
                sf::Vector2f{go.attribute("x").as_float(),go.attribute("y").as_float()},
                sf::degrees(go.attribute("r").as_float()),
                sf::Vector2f{go.attribute("sx").as_float(),go.attribute("sy").as_float()}
                );
    for (const auto& c : go.children())
    {
        ptr->addComponent(build_component(c));
    }
    return std::move(ptr);
}

void SceneManager::loadScene(std::string_view scene, std::vector<std::unique_ptr<GameObject>>& targets)
{
    std::filesystem::path path = scenes_dir / scene;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result) {
        std::cerr << "Could not open file visage.xml because " << result.description() << std::endl;
    }

    pugi::xml_node sceneObj = doc.child("Scene");
    for (const auto& go : sceneObj)
    {
        std::string name = go.name();
        if ( name != "GameObject") continue;
        targets.push_back(build_go(go));
    }
}

void SceneManager::unloadScene(std::vector<std::unique_ptr<GameObject>>& targets)
{
    targets.clear();
}
