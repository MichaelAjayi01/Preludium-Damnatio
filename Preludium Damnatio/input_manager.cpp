#include "input_manager.h"
#include <limits>  // For std::numeric_limits
#include <cstdlib> // For std::system

// Clear the console screen
void InputManager::ClearConsole() {
#ifdef _WIN32
    std::system("cls"); // Clear console for Windows
#else
    std::system("clear"); // Clear console for Unix/Linux/Mac
#endif
}

// Get player choice from a list of options
int InputManager::GetPlayerChoice(int optionsCount) {
    int choice = -1;
    while (true) {
        std::cout << "Enter your choice (1-" << optionsCount << "): ";
        std::cin >> choice;

        // Validate input
        if (std::cin.fail() || choice < 1 || choice > optionsCount) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard any leftover input
            ClearConsole(); // Clear the console after valid input
            break; // Valid choice, exit loop
        }
    }
    return choice;
}

// Get a string input from the player
std::string InputManager::GetStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input); // Get the entire line as input
    ClearConsole(); // Clear the console after getting the input
    return input;
}
