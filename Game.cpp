#include "Game.h"

#include <string>

#include "Components/Renderer.h"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Input/InputManager.h"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/SceneManager.h"
#include "Managers/UI/UIManager.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() {
  assert(mFont.openFromFile("resources/fonts/Sansation.ttf"));
  // We do not need to do mStatisticsText.setFont(mFont); as mStatisticsText is
  // initialized with a reference to mFont
  mStatisticsText.setPosition({5.f, 5.f});
  mStatisticsText.setCharacterSize(10);

  InputManager::init();

  scene=std::make_unique<Scene>();
  SceneManager::init(scene.get());
  RenderManager::init(&mWindow);
  SceneManager::requestLoading("resources/scenes/scene.xml");
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

    updateStatistics(elapsedTime);
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
  CollisionsManager::debugDraw(mWindow);
  UIManager::renderAll(mWindow);

  //mWindow.draw(mStatisticsText);
  mWindow.display();
}

void Game::updateStatistics(const sf::Time elapsedTime) {
  mStatisticsUpdateTime += elapsedTime;
  mStatisticsNumFrames += 1;

  if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
    mStatisticsText.setString(std::format(
        "Frames / Second = {}\nTime / Update = {} us", mStatisticsNumFrames,
        mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames));

    mStatisticsUpdateTime -= sf::seconds(1.0f);
    mStatisticsNumFrames = 0;
  }
}
