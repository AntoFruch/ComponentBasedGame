//
// Created by Antonin Fruchet on 09/06/2026.
//

#include "UIDocument.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
    const bool registered = []() {
        ComponentFactory::Register("UIDocument", [](const pugi::xml_node& node) {
            return std::make_unique<UIDocument>();
        });
        return true;
    }();
}
// --------------------------

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


