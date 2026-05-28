//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
#define COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
#include <string>
#include <vector>
#include <unordered_map>

enum ParamType { Bool, Trigger, Float };

struct Parameter
{
    ParamType type;
    std::variant<bool, float> value;
    std::variant<bool, float> getValue();
};

enum Comparaison{Equals, Less, Greater};
struct Condition
{
    const std::string parameterName;
    const std::variant<bool, float> cmp_val;
    const Comparaison cmp;
    [[nodiscard]] bool evaluate(std::unordered_map<std::string, Parameter>& parameters) const;
};

struct Transition
{
    const std::string target;

    const std::vector<Condition> conditions;
    [[nodiscard]] bool check(std::unordered_map<std::string, Parameter>& parameters) const;


};

struct AnimationState
{
    const std::string name;
    const unsigned int startFramex;
    const unsigned int startFramey;
    const unsigned int length;
    const unsigned int frameDuration; // ms
    const bool loop;
    const bool breakable;
    const std::vector<Transition> transitions;
};

struct AnimatorGraph {
    const std::string entry;

    const std::unordered_map<std::string, AnimationState> states;
    const std::unordered_map<std::string, Parameter> parameters;
};



#endif //COMPONENT_BASED_ARCH_ANIMATORGRAPH_H
