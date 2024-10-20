// story_manager.h

#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include <string>
#include <vector>
#include <map>

class StoryNode {
public:
    std::string text; // The narrative text for this node
    std::vector<std::string> options; // Player options leading to other nodes
    std::map<int, std::string> nextNodes; // Maps option index to the next node's identifier
    std::string asciiArt; // Filename for ASCII art associated with this node
    std::string audioFile; // Filename for audio associated with this node

    // Default constructor
    StoryNode() = default;

    // Constructor with parameters
    StoryNode(const std::string& text, const std::vector<std::string>& options,
        const std::map<int, std::string>& nextNodes,
        const std::string& asciiArt = "", const std::string& audioFile = "")
        : text(text), options(options), nextNodes(nextNodes), asciiArt(asciiArt), audioFile(audioFile) {}
};


class StoryManager {
public:
    StoryManager();

    // Load the story structure
    void LoadStory();

    // Display the current node's text
    void DisplayCurrentNode();

    // Handle player choice and update the current node
    void HandleChoice(int choice);

    // Get current options
    const std::vector<std::string>& GetCurrentOptions() const;

    // Get current ASCII art file
    const std::string& GetCurrentAsciiArt() const;

    // Get current audio file
    const std::string& GetCurrentAudio() const;

    // Check if the current node needs ASCII art
    bool NeedsAsciiArt() const;

    // Check if the current node needs audio
    bool NeedsAudio() const;

private:
    std::map<std::string, StoryNode> storyNodes; // Map of all story nodes
    std::string currentNode; // Identifier for the current node
};

#endif // STORY_MANAGER_H
