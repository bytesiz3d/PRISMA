#ifndef INPUT_HPP
#define INPUT_HPP

#include <set>
#include "user_input.hpp"


class InputManager {
    static bool compare(const UserInput* a, const UserInput* b);
    static std::set<const UserInput*, decltype(compare)*> set;
public:
    static void add(const UserInput* input);
    static void remove(const UserInput* input);

    [[nodiscard]] static GameInputData ProcessGameInput();

    [[nodiscard]] static MainMenuInputData ProcessMainMenuInput();
};

#endif //INPUT_HPP
