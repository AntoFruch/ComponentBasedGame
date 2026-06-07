//
// Created by Antonin Fruchet on 07/06/2026.
//

#include "UIElement.h"

#include "Managers/Scene/ComponentFactory.h"

// --- ENREGISTREMENT AUTOMATIQUE ---
// On crée une variable globale/statique anonyme.
// Son seul but est de s'exécuter AVANT le début du jeu pour enregistrer le composant.
namespace {
const bool registered = []() {
  ComponentFactory::Register("UIElement", [](const pugi::xml_node& node) {
      return std::make_unique<UIElement>();
  });
  return true;
}();
}
// --------------------------
UIElement::UIElement() {}
void UIElement::draw(sf::RenderWindow &window) {
  // do someting maybe
}
