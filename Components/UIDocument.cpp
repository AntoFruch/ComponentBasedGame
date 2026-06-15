//
// Created by Antonin Fruchet on 09/06/2026.
//

#include "UIDocument.h"

UIDocument::~UIDocument()
{
    gui->removeAllWidgets();
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

