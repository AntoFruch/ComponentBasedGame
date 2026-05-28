//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATOR_H
#define COMPONENT_BASED_ARCH_ANIMATOR_H
#include "AnimatorGraph.h"
#include "../Component.h"
#include "../Renderer.h"


class Animator : public Component {
    Renderer* renderer;
    const AnimatorGraph* graph;

    const AnimationState* m_currentState;
    std::unordered_map<std::string, Parameter> m_parameters;

    unsigned int currentFrame_x;
    unsigned int currentFrame_y;

    sf::Time m_timeCounter;

public:
    Animator(const std::string& animationTree_path);
    ~Animator() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void setParam(const std::string label, bool value);

private:
    void applyToRenderer();
    void makeTransition();
    void switchFrame();
};



#endif //COMPONENT_BASED_ARCH_ANIMATOR_H
