#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <string>

class InputManager {
public:
    int GetPlayerChoice(int optionsCount);

    std::string GetStringInput(const std::string& prompt);

    void ClearConsole();

private:

};

#endif