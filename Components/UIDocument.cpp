//
// Created by Antonin Fruchet on 09/06/2026.
//

#include "UIDocument.h"

UIDocument::~UIDocument()
{
    for (const auto& el: elements)
    {
        gui->remove(el);
    }
}

void UIDocument::Start()
{
    Component::Start();
    gui = SceneManager::getGui();
}
void UIDocument::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
}

void UIDocument::addElement(const tgui::Widget::Ptr& widget, const std::string& name)
{
    elements.push_back(widget);
    gui->add(widget, name);
}

void UIDocument::hide()
{
    hidden=true;
    for (auto& el : elements)
    {
        el->setVisible(false);
        el->setEnabled(false);
    }
}

void UIDocument::show()
{
    hidden=false;
    for (auto& el : elements)
    {
        el->setVisible(true);
        el->setEnabled(true);
    }
}

void UIDocument::moveToBack()
{
    for (auto& el:elements)
    {
        el->moveToBack();
    }
}

void UIDocument::moveToFront()
{
    for (auto& el:elements)
    {
        el->moveToFront();
    }
}
