#ifndef GAMEPAD_CONTROLLER_HPP
#define GAMEPAD_CONTROLLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "user_input.hpp"

class GamepadControllerInput: public UserInput {
    int id;

    static GamepadControllerInput instance; // Static instance
    GamepadControllerInput(); // Private constructor

    static void ConnectionCallback(int jid, int event);

    static glm::vec2 getWindowResolution() ;

    static glm::vec2 getCameraDelta(const GLFWgamepadstate &state);

    static glm::vec2 getPlayerMovement(const GLFWgamepadstate &state) ;

    [[nodiscard]] static GameInputData ProcessGamepadInput(const GLFWgamepadstate &state) ;


public:
    GamepadControllerInput(const GamepadControllerInput&) = delete;
    GamepadControllerInput& operator=(const GamepadControllerInput&) = delete;

    [[nodiscard]] bool isControllerConnected() const;

    [[nodiscard]] const char *getName() const;

    [[nodiscard]] GameInputData ProcessGameInput() const override;

    [[nodiscard]] MainMenuInputData ProcessMainMenuInput() const override {
        return {};
    }
};

#endif //GAMEPAD_CONTROLLER_HPP
