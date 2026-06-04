//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "Managers/Animation/AnimatorTreeLoader.h"
#include <iostream>
#include "exceptions/IllegalOperationException.h"

std::unordered_map<std::string, std::unique_ptr<AnimatorGraph>> AnimatorTreeLoader::parsedTrees{};

std::variant<bool, float> AnimatorTreeLoader::parseConditionValue(const pugi::xml_attribute& value)
{
    const std::string valueText = value.as_string();
    if (valueText == "true") return true;
    if (valueText == "false") return false;
    return value.as_float();
}

void AnimatorTreeLoader::loadConditions(const pugi::xml_node& cond, std::vector<Condition>& conditions)
{
    std::string name = cond.name();
    if (name != "Condition")
    {
        throw IllegalOperationException("Transition node children must be named \"Condition\"");
    }
    std::string cmp = cond.attribute("cmp").as_string();
    if (cmp == "Less")
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Less);
    }
    else if (cmp == "Greater")
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Greater);
    }
    else // default is Equals
    {
        conditions.emplace_back(cond.attribute("parameter").as_string(),
                                parseConditionValue(cond.attribute("value")), Equals);
    }
}

Transition AnimatorTreeLoader::createTransition(const pugi::xml_node& transi)
{
    std::vector<Condition> conditions;
    for (const auto& cond : transi.children())
    {
        loadConditions(cond, conditions);
    }
    return {
        transi.attribute("to").as_string(),
        conditions
    };
}

AnimationState AnimatorTreeLoader::createAnimationState(const pugi::xml_node& state)
{
    std::vector<Transition> transitions;
    for (const auto& transi : state.children())
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
        state.attribute("starty").as_int(),
        state.attribute("length").as_uint(),
        state.attribute("frameDuration").as_uint(),
        state.attribute("loop").as_bool(),
        state.attribute("breakable").as_bool(),
        transitions
    };
}

std::unordered_map<std::string, Parameter> AnimatorTreeLoader::createParametersMap(const pugi::xml_node& params)
{
    std::unordered_map<std::string, Parameter> parameters;
    for (const auto& param : params.children())
    {
        std::string type = param.attribute("type").as_string();
        if (type == "Trigger")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Trigger, param.attribute("default").as_bool()});
        }
        else if (type == "Bool")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Bool, param.attribute("default").as_bool()});
        }
        else if (type == "Float")
        {
            parameters.try_emplace(param.attribute("label").as_string(),
                                   Parameter{Float, param.attribute("default").as_float()});
        }
    }
    return parameters;
}

std::string AnimatorTreeLoader::retrieveEntryState(const pugi::xml_node& root_node)
{
    pugi::xml_node entry_node = root_node.child("EntryState");
    if (!entry_node) throw IllegalOperationException("An AnimationTree must have a \"EntryState\"");
    std::string label = entry_node.attribute("label").as_string();
    if (label.empty()) throw IllegalOperationException("EntryState \"label\" attribute must be unempty");
    return label;
}

BlendTree AnimatorTreeLoader::handleBlendTreeNode(const pugi::xml_node& state,
    std::unordered_map<std::string, AnimationState>& states)
{
    const std::string blendTreeLabel = state.attribute("label").as_string();
    if (blendTreeLabel.empty()) throw IllegalOperationException("BlendTree node must have a \"label\" attribute");

    const std::string path = state.attribute("src").as_string();
    if (path.empty()) throw IllegalOperationException("BlendTree node must have a \"src\" attribute");

    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        std::cerr << "Could not open AnimatorTree file because " << result.description() << std::endl;
        throw IllegalOperationException("Could not open AnimatorTree file");
    }
    auto tree_node = doc.child("BlendTree");
    if (!tree_node)
    {
        throw IllegalOperationException("Root node of BlendTree must be named \"BlendTree\"");
    }

    std::vector<Transition> externalTransitions;
    for (const auto& transi : state.children("Transition"))
    {
        externalTransitions.push_back(createTransition(transi));
    }

    std::vector<std::string> animationNames;
    std::vector<Transition> blendTreeTransitions;
    for (const auto& branch : tree_node.children())
    {
        if (const std::string name  = branch.name(); name!="Animation") continue;

        const std::string branchLabel = branch.attribute("label").as_string();
        animationNames.emplace_back(branchLabel);

        std::vector<Condition> conditions;
        for (const auto& cond : branch.children("Condition"))
        {
            loadConditions(cond, conditions);
        }
        blendTreeTransitions.emplace_back(branchLabel, conditions);
    }

    for (const auto& branch : tree_node.children())
    {
        if (const std::string name  = branch.name(); name!="Animation") continue;

        std::vector<Transition> transitions;
        for (const Transition& transition : externalTransitions)
        {
            transitions.push_back(transition);
        }

        for (const auto& branch2 : tree_node.children())
        {
            if (const std::string name = branch2.name(); name != "Animation") continue;

            const std::string label = branch.attribute("label").as_string();
            const std::string label2 = branch2.attribute("label").as_string();
            if (label2 == label) continue;

            for (const Transition& blendTreeTransition : blendTreeTransitions)
            {
                if (blendTreeTransition.target == label2)
                {
                    transitions.push_back(blendTreeTransition);
                    break;
                }
            }
        }

        states.try_emplace(
            branch.attribute("label").as_string(),
            branch.attribute("label").as_string(),
            branch.attribute("startx").as_uint(),
            branch.attribute("starty").as_int(),
            branch.attribute("length").as_uint(),
            branch.attribute("frameDuration").as_uint(),
            branch.attribute("loop").as_bool(),
            branch.attribute("breakable").as_bool(),
            transitions
        );
    }

    return {blendTreeLabel, animationNames, blendTreeTransitions};
}

std::unordered_map<std::string, AnimationState> AnimatorTreeLoader::createAnimationsMap(
    const pugi::xml_node& root_node,
    std::unordered_map<std::string, BlendTree>& blendTrees)
{
    auto tree_node = root_node.child("Tree");
    if (!tree_node) throw IllegalOperationException("AnimationTree must have a node named \"Tree\"");
    std::unordered_map<std::string, AnimationState> states;
    for (const auto& state : tree_node)
    {
        const std::string state_name = state.name();
        if (state_name == "Animation")
        {
            states.try_emplace(state.attribute("label").as_string(), createAnimationState(state));
        } else if (state_name == "BlendTree")
        {
            BlendTree blendTree = handleBlendTreeNode(state, states);
            blendTrees.try_emplace(blendTree.name, blendTree);
        } else
        {
            throw IllegalOperationException(R"(Each child node of "Tree" node must be named "Animation" or "BlendTree")");
        }
    }
    return states;
}

AnimatorGraph* AnimatorTreeLoader::loadAnimatorTree(const std::string& path)
{
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        std::cerr << "Could not open AnimatorTree file because " << result.description() << std::endl;
        throw IllegalOperationException("Could not open AnimatorTree file");
        return nullptr;
    }
    auto root_node = doc.child("AnimationTree");
    if (!root_node)
    {
        throw IllegalOperationException("Root node of AnimationTree must be named \"AnimationTree\"");
    }
    if (const std::string root_node_label = root_node.attribute("label").as_string(); root_node_label.empty())
    {
        throw IllegalOperationException("AnimationTree \"label\" attribute must not be empty");
    }

    // on cherche si cet AnimationTree a deja ete parsé et stocké, si oui on retourne un pointeur nu dessus.
    // c'est juste une structure de donnée qui est constante donc pas de probleme pour etre
    // lu par plusieurs Animators.
    const std::string root_node_label = root_node.attribute("label").as_string();
    if (auto it = parsedTrees.find(root_node_label); it != parsedTrees.end())
    {
        AnimatorGraph* raw_ptr = it->second.get();
        return raw_ptr;
    }

    std::unordered_map<std::string, BlendTree> blendTrees;
    auto ptr = std::make_unique<AnimatorGraph>(
        retrieveEntryState(root_node),
        createAnimationsMap(root_node, blendTrees),
        blendTrees,
        createParametersMap(root_node.child("Parameters"))
    );
    auto raw_ptr = ptr.get();
    parsedTrees[root_node_label] = std::move(ptr);
    return raw_ptr;
}
