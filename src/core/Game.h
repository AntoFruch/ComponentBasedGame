//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAME_H
#define COMPONENT_BASED_ARCH_GAME_H
#include "Assets/GameObject.h"


class Game {
public:
    Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    void run();

    static constexpr int W_WIDTH{640};
    static constexpr int W_HEIGHT{480};
    static constexpr float RATIO{W_WIDTH/W_HEIGHT};

    static constexpr int NB_TARGETS_INITIAL{20};

private:
    void processEvents();
    void update(sf::Time elapsedTime);
    void render();

    void updateStatistics(sf::Time elapsedTime);

    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow{sf::VideoMode({W_WIDTH, W_HEIGHT}), "SFML Application"};
    sf::Texture mTexture;
    std::vector<std::unique_ptr<GameObject>> mTargets;
    std::vector<Renderer*> mRenderers;
    sf::Font mFont;
    sf::Text mStatisticsText{mFont};
    sf::Time mStatisticsUpdateTime;

    std::size_t mStatisticsNumFrames{0};
};



#endif //COMPONENT_BASED_ARCH_GAME_H
