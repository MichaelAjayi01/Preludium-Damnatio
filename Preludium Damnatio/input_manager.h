#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <string>

class InputManager {
public:
    // Get player choice from a list of options
    int GetPlayerChoice(int optionsCount);

    // Get a string input from the player
    std::string GetStringInput(const std::string& prompt);
};

#endif
