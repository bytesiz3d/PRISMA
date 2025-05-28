#include "keyboard_mouse_input.hpp"

#include "input_manager.hpp"

KeyboardMouseInput KeyboardMouseInput::instance; // Static instance

KeyboardMouseInput::KeyboardMouseInput() : UserInput(Priority::NORMAL) {
    // Register this input handler with the InputManager
    InputManager::add(this);
}

glm::vec2 KeyboardMouseInput::getCameraPosition(GLFWwindow* window) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {x, y};
}

glm::vec2 KeyboardMouseInput::getPlayerMovement(GLFWwindow* window) {

    glm::vec2 movementPositive(0);
    glm::vec2 movementNegative(0);

    movementPositive.y = glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP);
    movementNegative.y = glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN);
    movementPositive.x = glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT);
    movementNegative.x = glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT);

    return movementPositive - movementNegative;
}

glm::vec2 KeyboardMouseInput::getCameraDelta(GLFWwindow* window) {
    static glm::vec2 lastPos = getCameraPosition(window);

    const glm::vec2 currentPos = getCameraPosition(window);
    const glm::vec2 delta = currentPos - lastPos;

    lastPos = currentPos;
    return delta;
}

GameInputData KeyboardMouseInput::ProcessGameInput() const {
    static bool oldSwapColors = false;
    GLFWwindow* window = glfwGetCurrentContext();

    bool exit = glfwGetKey(window, GLFW_KEY_ESCAPE);
    bool swapColors = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

    const glm::vec2 cameraDelta = getCameraDelta(window);
    const glm::vec2 playerMovement = getPlayerMovement(window);

    if (swapColors && oldSwapColors) {
        swapColors = false;
    } else {
        oldSwapColors = swapColors;
    }

    return {exit, swapColors, cameraDelta, playerMovement};
}

MainMenuInputData KeyboardMouseInput::ProcessMainMenuInput() const {
    int level = -1;

    GLFWwindow* window = glfwGetCurrentContext();

    bool exit = glfwGetKey(window, GLFW_KEY_ESCAPE);

    if (glfwGetKey(window, GLFW_KEY_1)) {
        level = 1;
    } else if (glfwGetKey(window, GLFW_KEY_2)) {
        level = 2;
    }

    return {exit, level};
}
