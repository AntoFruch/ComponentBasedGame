//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAME_H
#define COMPONENT_BASED_ARCH_GAME_H
#include "Assets/GameObject.h"
#include "Assets/Components/Renderer.h"


class Game {
public:
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    void run();

    static constexpr int W_WIDTH{640};
    static constexpr int W_HEIGHT{480};

private:

    void processEvents();
    void update(sf::Time elapsedTime);
    void render();

    void updateStatistics(sf::Time elapsedTime);

    bool mRunning;

    static const sf::Time TimePerFrame;

    std::vector<std::unique_ptr<GameObject>> mTargets;
    sf::Font mFont;
    sf::Text mStatisticsText{mFont};
    sf::Time mStatisticsUpdateTime;

    std::size_t mStatisticsNumFrames{0};

    sf::RenderWindow mWindow{sf::VideoMode({W_WIDTH, W_HEIGHT}), "SFML Application"};
};



#endif //COMPONENT_BASED_ARCH_GAME_H
