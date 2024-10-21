#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include <string>
#include <vector>
#include <map>

// StoryNode class representing a single node in the story
class StoryNode {
public:
    std::string text;
    std::vector<std::string> options;
    std::map<int, std::string> nextNodes;
    std::string asciiArt;
    std::string audioFile;

    // Default constructor
    StoryNode() = default;

    // Constructor with parameters
    StoryNode(const std::string& text, const std::vector<std::string>& options,
        const std::map<int, std::string>& nextNodes,
        const std::string& asciiArt = "", const std::string& audioFile = "")
        : text(text), options(options), nextNodes(nextNodes), asciiArt(asciiArt), audioFile(audioFile) {}
};

// StoryManager class for managing the flow of the story
class StoryManager {
public:
    StoryManager();

    void LoadStory();

    void DisplayCurrentNode();

    void HandleChoice(int choice);

    const std::vector<std::string>& GetCurrentOptions() const;

    const std::string& GetCurrentAsciiArt() const;

    const std::string& GetCurrentAudio() const;

    bool NeedsAsciiArt() const;

    bool NeedsAudio() const;

    void HandleRandomEncounter();

private:
    std::map<std::string, StoryNode> storyNodes; // All story nodes mapped by their identifier
    std::string currentNode; // Identifier for the current node

    int plotPointCounter; // Counter for tracking progress towards key plot points

    std::vector<std::string> randomNodes; // List of identifiers for random encounter nodes

    // Helper method to determine if the next encounter should be random or a key plot point
    bool IsKeyPlotPoint() const;
};

#endif // STORY_MANAGER_H
