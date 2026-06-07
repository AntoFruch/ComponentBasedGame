//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATOR_H
#define COMPONENT_BASED_ARCH_ANIMATOR_H
#include "Managers/Animation/AnimatorGraph.h"
#include "Component.h"
#include "Renderer.h"
using EventCallback = std::function<void()>;

struct AnimationEvent {
    const std::string animationState;
    const unsigned int triggerFrame;
    const EventCallback callback;
};

class Animator : public Component {
    Renderer* renderer;
    const AnimatorGraph* graph;

    const AnimationState* m_currentState;
    std::unordered_map<std::string, Parameter> m_parameters;
    std::unordered_map<std::string, AnimationEvent> m_animationEvents;

    unsigned int currentFrame_x;
    unsigned int currentFrame_y;

    sf::Time m_timeCounter;

public:
    Animator(const std::string& animationTree_path);
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void setParam(const std::string& label, const std::variant<bool, float>& value);
    void registerAnimationEvent(const std::string& animationState, unsigned int triggerFrame, EventCallback cbFunc);


private:
    void applyToRenderer();
    void checkForAndApplyEvent();
    void makeTransition();
    const AnimationState* resolveState(const std::string& target);
    void switchFrame();
};



#endif //COMPONENT_BASED_ARCH_ANIMATOR_H
