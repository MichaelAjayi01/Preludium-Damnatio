#include "story_manager.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

StoryManager::StoryManager()
    : currentNode("start"),
    plotPointCounter(0),
    encounterCounter(0)
{
    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}


// Load the story structure
void StoryManager::LoadStory() {
    // Key story nodes (fixed plot points)
    storyNodes["start"] = StoryNode(
        "After many days and many encounters, you reach the Lich King's doors. The air hangs heavy with the smell of death, thick with the weight of ancient curses. The towering obsidian gates before you creak and groan, their dark surface etched with symbols of forbidden power, pulsing faintly with an eerie, green glow. The shadows around you twist and writhe, as if alive, whispering forgotten horrors.\n\nThe ground trembles beneath your feet, as though the land itself recoils in fear of what lies beyond. Every step closer drains warmth from your bones, and the chilling laughter of the Lich King echoes faintly from within. His throne room awaits... and with it, the final choice: face your damnation, or forsake your very soul for the power to overthrow him.\n\nThe time has come. Will you step through the threshold of death, or turn back and embrace the madness that brought you here?",
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
		"An undead knight raises his sword at you..",
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
    const StoryNode& node = storyNodes[currentNode];
    std::cout << node.text << std::endl;

    for (size_t i = 0; i < node.options.size(); ++i) {
        std::cout << (i + 1) << ": " << node.options[i] << std::endl;
    }
}

void StoryManager::HandleChoice(int choice) {
    if (choice < 1 || choice > storyNodes[currentNode].options.size()) {
        throw std::out_of_range("Choice out of range");
    }

    bool isRandomEncounter = std::find(randomNodes.begin(), randomNodes.end(), currentNode) != randomNodes.end();

    if (isRandomEncounter) {
        // Increment encounterCounter for each random encounter
        encounterCounter++;
        currentNode = storyNodes[currentNode].nextNodes[choice - 1];

        // Display the current node
        DisplayCurrentNode();
        std::cout << "Encounter Counter: " << encounterCounter << std::endl;

        // Check if we reached the limit for encounters
        if (encounterCounter >= 3) {
            // Reset encounterCounter for future encounters
            encounterCounter = 0;

            // Handle the main encounter
            HandleMainEncounter();
            return; // Exit this function after handling the main encounter
        }
    }
    else if (IsKeyPlotPoint() && encounterCounter >= 3) {
        // Only allow main node transition after 3 random encounters
        currentNode = storyNodes[currentNode].nextNodes[choice - 1];
        plotPointCounter++;
        encounterCounter = 0; // Reset for future encounters
        DisplayCurrentNode();

        // Print encounter values after incrementing plotPointCounter
        std::cout << "Plot Point Counter: " << plotPointCounter << std::endl;
    }
    else {
        // Initiate a new random encounter
        HandleRandomEncounter();
    }
}

void StoryManager::HandleMainEncounter() {
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
    // In this example, every 3rd encounter is a key plot point
    return plotPointCounter % 3 == 0;
}
