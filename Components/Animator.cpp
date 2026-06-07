//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "Animator.h"

#include "Managers/Animation/AnimatorTreeLoader.h"
#include "GameObject.h"
#include "Managers/Scene/ComponentFactory.h"

#include <iostream>

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("Animator", [](const pugi::xml_node& node) {
            return std::make_unique<Animator>(node.attribute("src").as_string());
        });
        return true;
    }();
}
// --------------------------



Animator::Animator(const std::string& animationTree_path)
{
    graph = AnimatorTreeLoader::loadAnimatorTree(animationTree_path);
    m_parameters = graph->parameters;
    m_currentState = resolveState(graph->entry);
    m_timeCounter = sf::Time::Zero;
    currentFrame_x = m_currentState->startFramex;
    currentFrame_y = m_currentState->startFramey >= 0 ? m_currentState->startFramey : 0;
}

void Animator::Start()
{
    Component::Start();
    renderer = gameObject->getComponent<Renderer>();
}

void Animator::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    if (!m_currentState) return;

    m_timeCounter += elapsedTime;
    if (m_currentState->breakable) makeTransition();
    switchFrame();

    applyToRenderer();
}

void Animator::applyToRenderer()
{
    renderer->setCutRectPos(currentFrame_x,currentFrame_y);
}


void Animator::setParam(const std::string& label, const std::variant<bool, float>& value)
{
    if (m_parameters.contains(label))
    {
        m_parameters.at(label).value = value;
    }
}

void Animator::registerAnimationEvent(const std::string &animationState,
                                      unsigned int triggerFrame,
                                      EventCallback cbFunc) {
    m_animationEvents.try_emplace(animationState, animationState, triggerFrame, cbFunc);
}
void Animator::checkForAndApplyEvent() {
    // std::cout << "[Animator] State: " << m_currentState->name << " | Frame: " << currentFrame_x << "\n";

    if (m_animationEvents.contains(m_currentState->name)) {
        auto& event = m_animationEvents.at(m_currentState->name);
        if (event.triggerFrame == currentFrame_x - m_currentState->startFramex) {
        //    std::cout << "[Animator][EVENT] Triggered on state '" << m_currentState->name << "'"
        //              << " at frame " << currentFrame_x << "!\n";

            // Exécution du callback
            event.callback();
        }
    }
}

void Animator::makeTransition()
{
    if (!m_currentState->breakable && currentFrame_x-m_currentState->startFramex != m_currentState->length-1) return;
    for (const auto& transi : m_currentState->transitions)
    {

        if (transi.check(m_parameters))
        {
            m_currentState = resolveState(transi.target);
            //on se place au debut de l'animation
            currentFrame_x = m_currentState->startFramex;
            currentFrame_y = m_currentState->startFramey >= 0 ? m_currentState->startFramey : currentFrame_y;
            m_timeCounter = sf::Time::Zero;
            break;
        }
    }
}

const AnimationState* Animator::resolveState(const std::string& target)
{
    if (auto state = graph->states.find(target); state != graph->states.end())
    {
        return &state->second;
    }

    const BlendTree& blendTree = graph->blendTrees.at(target);
    for (const Transition& transition : blendTree.transitions)
    {
        if (transition.check(m_parameters))
        {
            return &graph->states.at(transition.target);
        }
    }

    return &graph->states.at(blendTree.children.front());
}

void Animator::switchFrame()
{
    if (m_timeCounter >= sf::milliseconds(m_currentState->frameDuration))
    {
        m_timeCounter -= sf::milliseconds(m_currentState->frameDuration); // reset en gardant le surplus pour rester en rythme a peu pres

        if (currentFrame_x < m_currentState->startFramex + m_currentState->length - 1)
        {
            ++currentFrame_x;
        } else if (m_currentState->loop)
        {
            currentFrame_x = m_currentState->startFramex;
        } else
        {
            makeTransition();
        }
        checkForAndApplyEvent();
    }
}
