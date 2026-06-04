//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATOR_H
#define COMPONENT_BASED_ARCH_ANIMATOR_H
#include "Managers/Animation/AnimatorGraph.h"
#include "Component.h"
#include "Renderer.h"


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
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void setParam(const std::string& label, const std::variant<bool, float>& value);

private:
    void applyToRenderer();
    void makeTransition();
    const AnimationState* resolveState(const std::string& target);
    void switchFrame();
};



#endif //COMPONENT_BASED_ARCH_ANIMATOR_H
