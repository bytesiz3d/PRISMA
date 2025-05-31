#include "input_manager.hpp"

bool InputManager::compare(const UserInput* a, const UserInput* b) {
    return *a < *b;
}

InputManager::SetType InputManager::set(&InputManager::compare);
std::unordered_map<const UserInput*, InputManager::SetType::iterator> InputManager::inputMap;

void InputManager::add(const UserInput* input) {
    if (!inputMap.contains(input)) {
        // If the input is not already in the set, insert it
        inputMap[input] = set.insert(input);
    }
}

void InputManager::remove(const UserInput* input) {
    if (!inputMap.contains(input)) {
        // If the input is not in the map, nothing to remove
        return;
    }
    // Remove the input from the set and the map
    const auto it = inputMap[input];
    inputMap.erase(input);
    set.erase(it);
}

GameInputData InputManager::ProcessGameInput() {
    if (set.empty()) {
        return {};
    }
    const auto& highestPriorityInput = *set.rbegin();
    return highestPriorityInput->ProcessGameInput();
}

MainMenuInputData InputManager::ProcessMainMenuInput(int levelsCount) {
    if (set.empty()) {
        return {};
    }
    const auto& highestPriorityInput = *set.rbegin();
    return highestPriorityInput->ProcessMainMenuInput(levelsCount);
}