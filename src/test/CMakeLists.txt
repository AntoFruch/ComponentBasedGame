#
# Lines inspired by https://google.github.io/googletest/quickstart-cmake.html
#

# Note: include(GoogleTest)and enable_testing() already done in top CMakeLists.txt

add_executable(unitTests
        unitTests.cpp
        sceneManagerTest.cpp
        inputManagerTest.cpp
        collisionsManagerTest.cpp
)
target_link_libraries(unitTests PUBLIC GTest::gtest_main lib_core pugixml sfml-graphics)

add_custom_command(TARGET unitTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:unitTests>/resources
)

# The next two lines enable CMake’s test runner to discover the tests included in the binary,
# using the GoogleTest CMake module.
include(GoogleTest)
gtest_discover_tests(unitTests)
