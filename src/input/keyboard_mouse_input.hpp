#ifndef KEYBOARD_MOUSE_INPUT_HPP
#define KEYBOARD_MOUSE_INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "user_input.hpp"

class KeyboardMouseInput: public UserInput {
    static KeyboardMouseInput instance;
    KeyboardMouseInput();

    static glm::vec2 getCameraPosition(GLFWwindow* window);

    static glm::vec2 getPlayerMovement(GLFWwindow* window);

    static glm::vec2 getCameraDelta(GLFWwindow* window);

public:
    KeyboardMouseInput(const KeyboardMouseInput&) = delete;
    KeyboardMouseInput& operator=(const KeyboardMouseInput&) = delete;

    [[nodiscard]] GameInputData ProcessGameInput() const override;

    [[nodiscard]] MainMenuInputData ProcessMainMenuInput() const override;
};

#endif //KEYBOARD_MOUSE_INPUT_HPP
