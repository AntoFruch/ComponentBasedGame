//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
#define COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
#include <string>

#include "AnimatorGraph.h"
#include "pugixml.hpp"


class AnimatorTreeLoader {
    static std::unordered_map<std::string , std::unique_ptr<AnimatorGraph>> parsedTrees;

    static std::unordered_map<std::string, AnimationState> createAnimationsMap(
        const pugi::xml_node& root_node,
        std::unordered_map<std::string, BlendTree>& blendTrees);
    static std::string retrieveEntryState(const pugi::xml_node& root_node);
    static std::unordered_map<std::string, Parameter> createParametersMap(const pugi::xml_node& params);
    static AnimationState createAnimationState(const pugi::xml_node& state);
    static BlendTree handleBlendTreeNode(const pugi::xml_node& state, std::unordered_map<std::string, AnimationState>& states);
    static Transition createTransition(const pugi::xml_node& transi);
    static void loadConditions(const pugi::xml_node& cond, std::vector<Condition>& conditions);
    static std::variant<bool, float> parseConditionValue(const pugi::xml_attribute& value);

public:
    static AnimatorGraph* loadAnimatorTree(const std::string& path);
};



#endif //COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
