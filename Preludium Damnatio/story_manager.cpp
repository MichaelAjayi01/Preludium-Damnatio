// story_manager.cpp
#include "story_manager.h"
#include <iostream>
#include <stdexcept>

// Constructor
StoryManager::StoryManager() : currentNode("start") {}

// Load the story structure
void StoryManager::LoadStory() {
    // Define story nodes (this could be replaced with loading from a file)
    storyNodes["start"] = StoryNode("You wake up in a dark room. What will you do?",
        { "Investigate the room", "Call for help" },
        { {0, "room_investigate"}, {1, "call_help"} });

    storyNodes["room_investigate"] = StoryNode("You find a strange book on the table. What next?",
        { "Read the book", "Leave the room" },
        { {0, "read_book"}, {1, "leave_room"} },
        "ascii_art/book.txt", "audio/book_read.wav");

    // Add more nodes as needed...
}

// Display the current node's text
void StoryManager::DisplayCurrentNode() {
    const StoryNode& node = storyNodes[currentNode];
    std::cout << node.text << std::endl;

    for (size_t i = 0; i < node.options.size(); ++i) {
        std::cout << (i + 1) << ": " << node.options[i] << std::endl;
    }
}

// Handle player choice and update the current node
void StoryManager::HandleChoice(int choice) {
    if (choice < 1 || choice > storyNodes[currentNode].options.size()) {
        throw std::out_of_range("Choice out of range");
    }

    // Update the current node based on the player's choice
    currentNode = storyNodes[currentNode].nextNodes[choice - 1];
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
