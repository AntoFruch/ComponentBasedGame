//
// Created by Antonin Fruchet on 26/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_INPUTACTION_H
#define COMPONENT_BASED_ARCH_INPUTACTION_H
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

#include "SFML/Window/Event.hpp"

struct DirectionalBindings
{
    sf::Keyboard::Key UP;
    sf::Keyboard::Key DOWN;
    sf::Keyboard::Key LEFT;
    sf::Keyboard::Key RIGHT;
};

struct ButtonBinding
{
    sf::Keyboard::Key key;
};

class InputAction {
    const std::string_view label;
    std::variant<bool, sf::Vector2f> m_value;

    std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>> bindings;
    std::set<sf::Keyboard::Key> pressed_keys;

public:
    explicit InputAction(const std::string& label, const std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>>& bindings);

    void processEvent(const std::optional<sf::Event>);

    template<typename T>
    T ReadValue() const;
};



#endif //COMPONENT_BASED_ARCH_INPUTACTION_H
