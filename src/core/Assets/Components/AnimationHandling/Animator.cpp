//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "Animator.h"

#include <iostream>

#include "AnimatorTreeLoader.h"
#include "Assets/GameObject.h"

Animator::Animator(const std::string& animationTree_path)
{
    graph = AnimatorTreeLoader::loadAnimatorTree(animationTree_path);
    m_currentState = &graph->states.at(graph->entry);
    m_parameters = graph->parameters;
    m_timeCounter = sf::Time::Zero;
}
Animator::~Animator()
{
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

void Animator::setParam(const std::string label, bool value)
{
    if (m_parameters.contains(label))
    {
        m_parameters.at(label).value = value;
    }
}

void Animator::makeTransition()
{
    if (!m_currentState->breakable){}
    for (const auto& transi : m_currentState->transitions)
    {

        if (transi.check(m_parameters))
        {
            m_currentState = &graph->states.at(transi.target);
            //on se place au debut de l'animation
            currentFrame_x = m_currentState->startFramex;
            m_timeCounter = sf::Time::Zero;
            break;
        }
    }
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
    }
}
