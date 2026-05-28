//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "AnimatorTreeLoader.h"

#include <iostream>
#include <pugixml.hpp>

std::unordered_map<std::string , std::unique_ptr<AnimatorGraph>> AnimatorTreeLoader::parsedTrees{};

Transition createTransition(const pugi::xml_node& transi)
{
    std::vector<Condition> conditions;
    for (const auto& cond : transi.children() )
    {
        std::string name = cond.name();
        if (name == "Condition")
        {
            conditions.emplace_back(cond.attribute("parameter").as_string(),
                                cond.attribute("value").as_bool() );
        }
    }
    return {
        transi.attribute("to").as_string(),
        conditions
    };
}

AnimationState createAnimationState(const pugi::xml_node& state)
{
    std::vector<Transition> transitions;
    for (const auto& transi : state.children() )
    {
        std::string name = transi.name();
        if (name == "Transition")
        {
            transitions.push_back(createTransition(transi));
        }
    }
    return {
        state.attribute("label").as_string(),
        state.attribute("startx").as_uint(),
        state.attribute("starty").as_uint(),
        state.attribute("length").as_uint(),
        state.attribute("frameDuration").as_uint(),
        state.attribute("loop").as_bool(),
        state.attribute("breakable").as_bool(),
        transitions
    };
}

AnimatorGraph* AnimatorTreeLoader::loadAnimatorTree(const std::string& path)
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result) {
        std::cerr << "Could not open Animator file because " << result.description() << std::endl;
        return nullptr;
    }
    auto root_node =  doc.child("AnimationTree");
    const std::string root_node_name = root_node.name();

    // on cherche si cet AnimationTree a deja ete parsé et stocké, si oui on retourne un pointeur nu dessus.
    // c'est juste une structure de donnée qui est constante donc pas de probleme pour etre
    // lu par plusieurs Animators.
    if (auto it = parsedTrees.find(root_node_name); it != parsedTrees.end()) {
        AnimatorGraph* raw_ptr = it->second.get();
        return raw_ptr;
    }

    // Parameters
    auto parameters_node = root_node.child("Parameters");

    std::unordered_map<std::string, Parameter> parameters;
    for (const auto& param : parameters_node.children())
    {
        std::string type = param.attribute("type").as_string();
        if (type=="Trigger")
        {
            parameters.try_emplace(param.attribute("label").as_string(), Parameter{Trigger,param.attribute("value").as_bool() }) ;
        } else if (type=="Bool")
        {
            parameters.try_emplace(param.attribute("label").as_string(), Parameter{Bool,param.attribute("value").as_bool() }) ;
        } else if (type == "Float")
        {
            parameters.try_emplace(param.attribute("label").as_string(), Parameter{Float,param.attribute("value").as_float() }) ;
        }

    }

    // Entry
    std::string entry_state = root_node.child("EntryState").attribute("label").as_string();

    // Actual Animations
    auto tree_node = root_node.child("Tree");
    std::unordered_map<std::string, AnimationState> states;
    for (const auto& state : tree_node)
    {
        states.try_emplace(state.attribute("label").as_string(), createAnimationState(state));
    }



    auto ptr = std::make_unique<AnimatorGraph>(
            entry_state,
            states,
            parameters
        );
    auto raw_ptr = ptr.get();
    parsedTrees[root_node_name] = std::move(ptr);
    return raw_ptr;
}

