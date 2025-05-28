#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <glm/glm.hpp>

struct GameInputData {
    bool exit = false;
    bool swapColors = false;
    glm::vec2 cameraMovement = glm::vec2{0};
    glm::vec2 playerMovement = glm::vec3{0};
};

struct MainMenuInputData {
    bool exit = false;
    bool startGame = false;
    int level = 1;
};

class UserInput {
protected:
    enum class Priority {
        VERY_LOW = -100,
        LOW = -50,
        NORMAL = 0,
        HIGH = 50,
        VERY_HIGH = 100,
    };

    Priority inputPriority;

public:
    [[nodiscard]] virtual GameInputData ProcessGameInput() const = 0;
    [[nodiscard]] virtual MainMenuInputData ProcessMainMenuInput() const = 0;

    explicit UserInput(const Priority priority): inputPriority(priority) {}
    virtual ~UserInput() = default;

    bool operator < (const UserInput& other) const {
        return inputPriority < other.inputPriority;
    }
};

#endif //USER_INPUT_HPP
