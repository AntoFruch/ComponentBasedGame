//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
#define COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
#include <string>
#include <vector>
#include <unordered_map>
struct Condition
{
    const std::string parameterName;
    const bool expected_val;
    [[nodiscard]] bool evaluate(const std::unordered_map<std::string, bool>& parameters) const;
};

struct Transition
{
    const std::string target;

    const std::vector<Condition> conditions;
    [[nodiscard]] bool check(const std::unordered_map<std::string, bool>& parameters) const;


};

struct AnimationState
{
    const std::string name;
    const unsigned int startFramex;
    const unsigned int startFramey;
    const unsigned int length;
    const unsigned int frameDuration; // ms
    const bool loop;
    const std::vector<Transition> transitions;
};

struct AnimatorGraph {
    const std::string entry;

    const std::unordered_map<std::string, AnimationState> states;
    const std::unordered_map<std::string, bool> parameters;
};



#endif //COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
