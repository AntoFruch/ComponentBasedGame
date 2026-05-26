//
// Created by Antonin Fruchet on 26/05/2026.
//

#include <gtest/gtest.h>

#include "InputManagement/InputManager.h"

TEST(InputManagerTest, ReadsInputConfigAndInitializesConfiguredActions)
{
    ASSERT_NO_THROW(InputManager::init());

    InputAction* moveAction = nullptr;
    EXPECT_NO_THROW(moveAction = InputManager::findAction("Move"));
    EXPECT_NE(moveAction, nullptr);

    InputAction* jumpAction = nullptr;
    EXPECT_NO_THROW(jumpAction = InputManager::findAction("Jump"));
    EXPECT_NE(jumpAction, nullptr);
}

TEST(InputActionTest, ProcessesButtonKeyPressedAndReleasedEvents)
{
    InputAction jumpAction{
        "Jump",
        std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>>{
            std::vector<ButtonBinding>{{sf::Keyboard::Key::Space}}
        }
    };

    EXPECT_FALSE(jumpAction.ReadValue<bool>());

    jumpAction.processEvent(sf::Event{sf::Event::KeyPressed{sf::Keyboard::Key::Space}});
    EXPECT_TRUE(jumpAction.ReadValue<bool>());

    jumpAction.processEvent(sf::Event{sf::Event::KeyReleased{sf::Keyboard::Key::Space}});
    EXPECT_FALSE(jumpAction.ReadValue<bool>());
}

TEST(InputActionTest, ProcessesDirectionalKeyPressedAndReleasedEvents)
{
    InputAction moveAction{
        "Move",
        std::variant<std::vector<ButtonBinding>, std::vector<DirectionalBindings>>{
            std::vector<DirectionalBindings>{{
                sf::Keyboard::Key::Z,
                sf::Keyboard::Key::S,
                sf::Keyboard::Key::Q,
                sf::Keyboard::Key::D
            }}
        }
    };

    EXPECT_EQ(moveAction.ReadValue<sf::Vector2f>(), sf::Vector2f({0, 0}));

    moveAction.processEvent(sf::Event{sf::Event::KeyPressed{sf::Keyboard::Key::Z}});
    EXPECT_EQ(moveAction.ReadValue<sf::Vector2f>(), sf::Vector2f({0, -1}));

    moveAction.processEvent(sf::Event{sf::Event::KeyPressed{sf::Keyboard::Key::D}});
    EXPECT_EQ(moveAction.ReadValue<sf::Vector2f>(), sf::Vector2f({1, -1}));

    moveAction.processEvent(sf::Event{sf::Event::KeyReleased{sf::Keyboard::Key::Z}});
    EXPECT_EQ(moveAction.ReadValue<sf::Vector2f>(), sf::Vector2f({1, 0}));

    moveAction.processEvent(sf::Event{sf::Event::KeyReleased{sf::Keyboard::Key::D}});
    EXPECT_EQ(moveAction.ReadValue<sf::Vector2f>(), sf::Vector2f({0, 0}));
}
