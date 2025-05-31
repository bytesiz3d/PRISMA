#ifndef INPUT_HPP
#define INPUT_HPP

#include <set>
#include <unordered_map>
#include "user_input.hpp"


class InputManager {
    static bool compare(const UserInput* a, const UserInput* b);

    typedef std::multiset<const UserInput*, decltype(compare)*> SetType;

    static SetType set;
    static std::unordered_map<const UserInput*, SetType::iterator> inputMap;
public:
    static void add(const UserInput* input);
    static void remove(const UserInput* input);

    [[nodiscard]] static GameInputData ProcessGameInput();

    [[nodiscard]] static MainMenuInputData ProcessMainMenuInput(int levelsCount);
};

#endif //INPUT_HPP
