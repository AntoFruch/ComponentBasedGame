#include "Game.h"

#include <objc/objc.h>

#include <random>
#include <string>

#include "Assets/Components/Renderer.h"
#include "InputManagement/InputManager.h"
#include "Managers/RenderManager.h"
#include "SceneManagement/SceneManager.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() {
  mRunning=false;
  assert(mFont.openFromFile("resources/fonts/Sansation.ttf"));
  // We do not need to do mStatisticsText.setFont(mFont); as mStatisticsText is
  // initialized with a reference to mFont
  mStatisticsText.setPosition({5.f, 5.f});
  mStatisticsText.setCharacterSize(10);

  // TESTING
  InputManager::init();
  SceneManager::loadScene("scene.xml", mTargets);
  for (const auto& go : mTargets)
  {
    go->start();
  }

  RenderManager::handleResize(mWindow.getSize());
}

void Game::run() {
  if (mRunning) return;
  mRunning = true;
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
  mRunning=false;
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
  for (auto& target : mTargets)
  {
    target->update(elapsedTime);
  }
}

void Game::render() {
  RenderManager::renderAll(mWindow);

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
