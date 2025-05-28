#include "input_manager.hpp"

bool InputManager::compare(const UserInput* a, const UserInput* b) {
    return *a < *b;
}

std::set<const UserInput*, decltype(InputManager::compare)*> InputManager::set(&InputManager::compare);

void InputManager::add(const UserInput* input) {
    set.insert(input);
}

void InputManager::remove(const UserInput* input) {
    set.erase(input);
}

GameInputData InputManager::ProcessGameInput() {
    if (set.empty()) {
        return {};
    }
    const auto& highestPriorityInput = *set.rbegin();
    return highestPriorityInput->ProcessGameInput();
}

MainMenuInputData InputManager::ProcessMainMenuInput() {
    if (set.empty()) {
        return {};
    }
    const auto& highestPriorityInput = *set.rbegin();
    return highestPriorityInput->ProcessMainMenuInput();
}