//
// Created by Antonin Fruchet on 07/06/2026.
//

#include "UIManager.h"

#include "Managers/Render/RenderManager.h"

std::vector<UIElement*> UIManager::elements{};

void UIManager::registerUIElement(UIElement *el) {
  elements.push_back(el);
}
void UIManager::unregisterUIElement(UIElement *el) {
  std::erase_if(elements,[&el](const UIElement* element){ return el==element;});
}

void UIManager::renderAll(sf::RenderWindow& window) {
  sf::View oldView{window.getView()};


  float windowRatio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
  auto newWidth = windowRatio * RenderManager::referenceHeight;

  sf::View uiView{sf::FloatRect{{0,0}, sf::Vector2f{newWidth, RenderManager::referenceHeight}}};

  window.setView(uiView);

  for ( const auto& el : elements) {
    el->draw(window);
  }

  window.setView(oldView);
}
