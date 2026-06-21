#include "Game.h"

#include <string>

#include "Components/Renderer.h"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Input/InputManager.h"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/SceneManager.h"
#include "TGUI/AllWidgets.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game(const std::string& scenePath) {
  InputManager::init();

  scene=std::make_unique<Scene>(mWindow);
  SceneManager::init(scene.get());
  RenderManager::init(&mWindow);

  SceneManager::requestLoading(scenePath);
}

void Game::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  mWindow.setVerticalSyncEnabled(true);
  while (mWindow.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;

      processEvents();
      update(TimePerFrame);
    }

    render();
  }
}

void Game::processEvents() {
  InputManager::beginFrame();
  
  while (const std::optional event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      mWindow.close();
    }
    if (const auto& resized = event->getIf<sf::Event::Resized>()) {
      RenderManager::handleResize(resized->size);
    }
    InputManager::processEvents(event);
  }
}

void Game::update(const sf::Time elapsedTime) {
  SceneManager::Update(elapsedTime);
}

void Game::render() {
  RenderManager::renderAll(mWindow);
#if defined(COLLISION_DEBUG)
  CollisionsManager::debugDraw(mWindow);
#endif
  mWindow.display();
}
