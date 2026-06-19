//
// Created by Antonin Fruchet on 09/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_UIDOCUMENT_H
#define COMPONENT_BASED_ARCH_UIDOCUMENT_H


#include "Engine.h"
#include "Managers/Scene/ComponentFactory.h"

class UIDocument : public Component {
    tgui::Gui* gui;


protected:
    bool hidden{false};
    std::vector<tgui::Widget::Ptr> elements;
    tgui::Font customFont{"resources/fonts/PixelifySans-VariableFont_wght.ttf"};
public:
    UIDocument()=default;
    ~UIDocument() override;
    void Start() override;
    void Update(const sf::Time& elapsedTime) override;
    void addElement(const tgui::Widget::Ptr& widget, const std::string& name);

    void virtual hide();
    void virtual show();


private:
    static inline bool s_registered = ComponentFactory::Register("UIDocument", [](const pugi::xml_node& node) -> std::unique_ptr<Component> {
        return std::make_unique<UIDocument>();
    });
};



#endif //COMPONENT_BASED_ARCH_UIDOCUMENT_H
