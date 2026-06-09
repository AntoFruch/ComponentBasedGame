//
// Created by Antonin Fruchet on 09/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_UIDOCUMENT_H
#define COMPONENT_BASED_ARCH_UIDOCUMENT_H


#include "Engine.h"

class UIDocument : public Component {
    tgui::Gui* gui;

protected:
    std::vector<tgui::Widget::Ptr> elements;
    tgui::Font customFont{"resources/fonts/PixelifySans-VariableFont_wght.ttf"};
public:
    UIDocument()=default;
    ~UIDocument() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void addElement(const tgui::Widget::Ptr& widget, const std::string& name);
};



#endif //COMPONENT_BASED_ARCH_UIDOCUMENT_H
