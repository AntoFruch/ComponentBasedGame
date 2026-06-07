//
// Created by Antonin Fruchet on 07/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_UIELEMENT_H
#define COMPONENT_BASED_ARCH_UIELEMENT_H
#include "Component.h"
#include "SFML/Graphics.hpp"

class UIElement : public Component{

public:
  UIElement();
  virtual void draw(sf::RenderWindow& window);
};



#endif //COMPONENT_BASED_ARCH_UIELEMENT_H
