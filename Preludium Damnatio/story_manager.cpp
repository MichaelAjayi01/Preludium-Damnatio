#include "story_manager.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

StoryManager::StoryManager(InputManager& inputManager, RenderManager& renderManager)
    : currentNode("start"),
    plotPointCounter(0),
    encounterCounter(0),
    inputManager(inputManager),
    renderManager(renderManager) // Correctly assigned reference
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}


// Load the story structure
void StoryManager::LoadStory() {
    // Key story nodes (fixed plot points)
    storyNodes["start"] = StoryNode(
        "\"Where...am I?\"",
        { "Proceed" },
        { {0, "selection_menu"} }
    );

    storyNodes["selection_menu"] = StoryNode(
        "You stand before three ancient doors, each carved with strange, foreboding symbols. To your left, a door of weathered stone, cracked and emanating the smell of damp earth. In the center, a door of gleaming gold, blinding in its brightness but humming with an unsettling energy. To your right, a door of twisted iron, blackened and seething with dark tendrils of smoke.\n\nWhispers fill the air around you, indecipherable but insistent. Your heart pounds as the weight of your decision presses upon you. Each door promises a different fate, but which one holds salvation, and which one seals your doom?",
        { "Enter the stone door", "Enter the golden door", "Enter the iron door" },
        { {0, "stone_room"}, {1, "golden_room"}, {2, "iron_room"} }
    );

    storyNodes["main_lich_king_encounter"] = StoryNode(
        "The Lich King stands before you, his skeletal gaze piercing into your soul. This is the moment of truth.",
        { "Fight", "Negotiate", "Flee" },
        { {0, "battle"}, {1, "negotiation"}, {2, "escape"} }
    );

    storyNodes["random_encounter_1"] = StoryNode(
        "You encounter a group of wandering undead. They eye you hungrily.",
        { "Fight", "Run" },
        { {0, "selection_menu"}, {1, "selection_menu"} }
    );

    storyNodes["random_encounter_2"] = StoryNode(
        "A strange figure offers you a deal. Will you accept the cursed power?",
        { "Accept", "Reject" },
        { {0, "selection_menu"}, {1, "selection_menu"} }
    );

    storyNodes["random_encounter_3"] = StoryNode(
        "You stumble upon a hidden chamber filled with ancient artifacts. Will you take them?",
        { "Take the artifacts", "Leave them" },
        { {0, "selection_menu"}, {1, "selection_menu"} }
    );

    storyNodes["random_encounter_4"] = StoryNode(
        "An undead knight raises his sword at you.",
        { "Fight", "Run" },
        { {0, "selection_menu"}, {1, "selection_menu"} }
    );

    storyNodes["random_encounter_5"] = StoryNode(
        "A Revenant stalks this room. Its accusatory glare meets yours...",
        { "Fight", "Run" },
        { {0, "selection_menu"}, {1, "selection_menu"} }
    );

    // Add more random encounters...
    randomNodes.push_back("random_encounter_1");
    randomNodes.push_back("random_encounter_2");
    randomNodes.push_back("random_encounter_3");
    randomNodes.push_back("random_encounter_4");
    randomNodes.push_back("random_encounter_5");
}

void StoryManager::DisplayCurrentNode() {
    // Fetch the current node to display
    const StoryNode& node = storyNodes[currentNode];

    // Debugging output: print the current node ID and its text
    std::cout << "Displaying Current Node: " << currentNode << std::endl;
    std::cout << "Node Text: " << node.text << std::endl;

    // Define a color for the text
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Define the maximum width for the text rendering
    const int maxWidth = 600;

    // Render the node's main text with wrapping
    renderManager.RenderTextToScreen(node.text, 10, 10, textColor, maxWidth); // Use the variable

    // Variable to keep track of the current y position for rendering options
    int optionYPosition = 50; // Start y position for the first option

    // Render each option with spacing, adjusting y-position for each line
    for (size_t i = 0; i < node.options.size(); ++i) {
        std::string optionText = std::to_string(i + 1) + ": " + node.options[i];
        std::cout << "Option " << i + 1 << ": " << optionText << std::endl; // Debugging output for each option

        // Render the option text with wrapping
        renderManager.RenderTextToScreen(optionText, 10, optionYPosition, textColor, maxWidth); // Use the variable
        optionYPosition += 30; // Increase y position for the next option
    }
}




void StoryManager::HandleChoice(int choice) {
    // Validate choice
    if (choice < 1 || choice > storyNodes[currentNode].options.size()) {
        throw std::out_of_range("Choice out of range");
    }

    bool isRandomEncounter = std::find(randomNodes.begin(), randomNodes.end(), currentNode) != randomNodes.end();

    if (isRandomEncounter) {
        // Increment encounter counter for random encounters
        encounterCounter++;

        // Move to the next node based on player's choice
        currentNode = storyNodes[currentNode].nextNodes[choice - 1].second; // Change here

        // Only display if it's not the third encounter
        if (encounterCounter < 3) {
            DisplayCurrentNode();
            // renderManager->RenderTextToScreen("Encounter Counter: " + std::to_string(encounterCounter), 10, 200, textColor);
        }

        // Check if the player has had 3 encounters
        if (encounterCounter >= 3) {
            encounterCounter = 0; // Reset counter for next set of encounters
            HandleMainEncounter(); // Move to a main encounter
            return;
        }
    }
    else if (IsKeyPlotPoint() && encounterCounter >= 3) {
        // Handle key plot point after three encounters
        currentNode = storyNodes[currentNode].nextNodes[choice - 1].second; // Change here
        plotPointCounter++;
        encounterCounter = 0; // Reset after transitioning to the main plot point
    }
    else {
        // Initiate a new random encounter
        HandleRandomEncounter();
    }
}

void StoryManager::HandleMainEncounter() {
    inputManager.ClearConsole();

    // Create a vector to hold all main encounter nodes
    std::vector<std::string> mainEncounters;

    // Iterate over the storyNodes to find those with "main" in their names
    for (const auto& pair : storyNodes) {
        if (pair.first.find("main") == 0) {
            mainEncounters.push_back(pair.first);
        }
    }

    // Select a random main encounter from the available options
    if (!mainEncounters.empty()) {
        int randomIndex = std::rand() % mainEncounters.size();
        currentNode = mainEncounters[randomIndex]; // Set currentNode to a random main encounter

        // Now display the current node
        DisplayCurrentNode();
    }
    else {
        std::cout << "No main encounters available!" << std::endl;
        // Fallback logic if no main encounters exist
        currentNode = "selection_menu"; // Fallback to selection menu
        DisplayCurrentNode();
    }
}

// Get current options
const std::vector<std::string>& StoryManager::GetCurrentOptions() const {
    return storyNodes.at(currentNode).options;
}

// Get current ASCII art file
const std::string& StoryManager::GetCurrentAsciiArt() const {
    return storyNodes.at(currentNode).asciiArt;
}

// Get current audio file
const std::string& StoryManager::GetCurrentAudio() const {
    return storyNodes.at(currentNode).audioFile;
}

// Check if the current node needs ASCII art
bool StoryManager::NeedsAsciiArt() const {
    return !storyNodes.at(currentNode).asciiArt.empty();
}

// Check if the current node needs audio
bool StoryManager::NeedsAudio() const {
    return !storyNodes.at(currentNode).audioFile.empty();
}

// Handle random encounter by selecting from the pool of random nodes
void StoryManager::HandleRandomEncounter() {
    int randomIndex = std::rand() % randomNodes.size();
    currentNode = randomNodes[randomIndex];

    DisplayCurrentNode();
}

// Helper method to check if the next encounter should be a key plot point
bool StoryManager::IsKeyPlotPoint() const {
    return plotPointCounter < 3; // Adjust logic based on your game requirements
}
