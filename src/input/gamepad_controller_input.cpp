#include "gamepad_controller_input.hpp"
#include "input_manager.hpp"

#include <chrono>
#include <iostream>

GamepadControllerInput GamepadControllerInput::instance;

// Private constructor to prevent instantiation
GamepadControllerInput::GamepadControllerInput(): UserInput(Priority::VERY_HIGH), id(-1) {
    // check if glfw  is initialized
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickIsGamepad(i)) {
            id = i; // Set the first connected gamepad as the current one
            std::cout << "Gamepad already connected: " << getName() << " (ID: " << id << ")\n";
        }
    }

    if (id >= GLFW_JOYSTICK_1) {
        InputManager::add(this); // Register this input handler with the InputManager
    }

    glfwSetJoystickCallback(ConnectionCallback);
}

void GamepadControllerInput::ConnectionCallback(const int jid, const int event) {
    if (event == GLFW_CONNECTED && glfwJoystickIsGamepad(jid)) {
        instance.id = jid;
        std::cout << "Joystick connected: " << instance.getName() << " (ID: " << jid << ")\n";
        InputManager::add(&instance); // Register this input handler with the InputManager
    } else if (event == GLFW_DISCONNECTED) {
        // Joystick was disconnected
        if (instance.id == jid) {
            instance.id = -1; // Reset ID if the disconnected joystick was the current one
            InputManager::remove(&instance); // Remove this input handler from the InputManager
        }
        std::cout << "Joystick disconnected: " << instance.getName() << " (ID: " << jid << ")\n";
    }
}

glm::vec2 GamepadControllerInput::getWindowResolution() {
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetWindowSize(window, &width, &height);
    return {width, height};
}

glm::vec2 GamepadControllerInput::getCameraDelta(const GLFWgamepadstate &state) {
    return {
        state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] / 100,
        state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] / 100
    };
}

glm::vec2 GamepadControllerInput::getPlayerMovement(const GLFWgamepadstate &state) {
    return {state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
}

GameInputData GamepadControllerInput::ProcessGamepadInput(const GLFWgamepadstate &state) {
    static bool oldSwapColors = false;

    bool swapColors = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
    bool exit = state.buttons[GLFW_GAMEPAD_BUTTON_BACK] || state.buttons[GLFW_GAMEPAD_BUTTON_START];

    const glm::vec2 cameraDelta = getCameraDelta(state);
    const glm::vec2 playerMovement = getPlayerMovement(state);

    if (swapColors && oldSwapColors) {
        swapColors = false;
    } else {
        oldSwapColors = swapColors;
    }

    return {exit, swapColors, cameraDelta, playerMovement};
}

bool GamepadControllerInput::isControllerConnected() const {
    if (id < GLFW_JOYSTICK_1 || id > GLFW_JOYSTICK_LAST) {
        return false;
    }
    return glfwJoystickIsGamepad(id);
}

const char * GamepadControllerInput::getName() const {
    if (!isControllerConnected()) {
        return "No Controller Connected";
    }
    return glfwGetGamepadName(id);
}

GameInputData GamepadControllerInput::ProcessGameInput() const {
    GLFWgamepadstate state;

    if (!isControllerConnected() || !glfwGetGamepadState(id, &state)) {
        return{};
    }

    return ProcessGamepadInput(state);
}

MainMenuInputData GamepadControllerInput::ProcessMainMenuInput(int levelsCount) const {
    using namespace std::chrono_literals;

    static int level = 1;
    static auto lastInputTime = std::chrono::steady_clock::now();
    auto cooldown = 200ms;

    GLFWgamepadstate state;

    if (!isControllerConnected() || !glfwGetGamepadState(id, &state)) {
        return{};
    }

    bool exit = state.buttons[GLFW_GAMEPAD_BUTTON_BACK] || state.buttons[GLFW_GAMEPAD_BUTTON_START];
    bool startGame = state.buttons[GLFW_GAMEPAD_BUTTON_A];

    auto now = std::chrono::steady_clock::now();

    if (now - lastInputTime > cooldown) {
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5f) {
            level = std::max(1, level - 1);
            lastInputTime = now;
        } else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.5f) {
            level = std::min(levelsCount, level + 1);
            lastInputTime = now;
        }
    }

    return {exit, startGame, level};
}
