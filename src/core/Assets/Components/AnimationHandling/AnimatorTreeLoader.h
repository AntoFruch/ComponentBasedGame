//
// Created by Antonin Fruchet on 27/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
#define COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
#include <string>

#include "AnimatorGraph.h"


class AnimatorTreeLoader {
    static std::unordered_map<std::string , std::unique_ptr<AnimatorGraph>> parsedTrees;
public:
    static AnimatorGraph* loadAnimatorTree(const std::string& path);
};



#endif //COMPONENT_BASED_ARCH_ANIMATORTREELOADER_H
