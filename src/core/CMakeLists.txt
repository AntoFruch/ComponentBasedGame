add_library(lib_core
        myMain.cpp
        myMain.h
        Assets/GameObject.cpp
        Assets/GameObject.h
        Assets/Components/Component.cpp
        Assets/Components/Component.h
        Assets/Components/Transform.cpp
        Assets/Components/Transform.h
        Game.cpp
        Game.h
        Assets/Components/Renderer.cpp
        Assets/Components/Renderer.h
        SceneManagement/SceneManager.cpp
        SceneManagement/SceneManager.h
        InputManagement/InputManager.cpp
        InputManagement/InputManager.h
        InputManagement/InputAction.cpp
        InputManagement/InputAction.h
        Assets/Components/Controller.cpp
        Assets/Components/Controller.h
)
target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC pugixml sfml-graphics)

