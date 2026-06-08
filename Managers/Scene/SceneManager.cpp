//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "SceneManager.h"

#include <iostream>


Scene* SceneManager::scene = nullptr;
bool SceneManager::loadingReq = false;
std::string SceneManager::reqPath = "";

void SceneManager::init(Scene* ptr)
{
    if (scene==nullptr)
    {
        scene = ptr;
    }
}

void SceneManager::Update(const sf::Time& elapsedTime)
{
    scene->Update(elapsedTime);
}

void SceneManager::requestLoading(std::string_view path)
{
    if (loadingReq) return;
    loadingReq = true;
    reqPath = path;
}

void SceneManager::applyRequest()
{
    scene->unload();
    scene->load(reqPath);
    std::string dump{scene->dump()};
    std::cout << dump << std::endl;
    scene->Start();
    loadingReq=false;
    reqPath = "";
}

GameObject* SceneManager::instantiate(std::string_view prefab)
{
    return scene->requestInstantiate(prefab);
}

tgui::Gui* SceneManager::getGui()
{
    return scene->getGui();
}
