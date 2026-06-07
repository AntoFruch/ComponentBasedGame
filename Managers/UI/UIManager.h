//
// Created by Antonin Fruchet on 07/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_UIMANAGER_H
#define COMPONENT_BASED_ARCH_UIMANAGER_H
#include "Components/UIElement.h"

class UIManager {
  static std::vector<UIElement*> elements;
public:
  static void registerUIElement(UIElement* el);
  static void unregisterUIElement(UIElement* el);

  static void renderAll(sf::RenderWindow& window);
};



#endif //COMPONENT_BASED_ARCH_UIMANAGER_H
