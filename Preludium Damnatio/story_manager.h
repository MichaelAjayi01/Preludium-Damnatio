#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

// Include necessary headers
#include "render_manager.h"
#include "input_manager.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class StoryNode {
public:
    std::string text;
    std::vector<std::string> options;
    std::vector<std::pair<int, std::string>> nextNodes;
    std::string asciiArt;
    std::string audioFile;
    std::string imageFile; // New member for image file

    // Default constructor
    StoryNode() = default;

    // Custom constructor
    StoryNode(const std::string& text, const std::vector<std::string>& options, const std::vector<std::pair<int, std::string>>& nextNodes, const std::string& imageFile = "")
        : text(text), options(options), nextNodes(nextNodes), imageFile(imageFile) {}
};

class StoryManager {
public:
    // Updated constructor to accept RenderManager reference
    StoryManager(InputManager& inputManager, RenderManager& renderManager);
    void LoadStory();
    void DisplayCurrentNode();
    void HandleChoice(int choice);

    // Public methods for accessing story details
    const std::vector<std::string>& GetCurrentOptions() const;
    const std::string& GetCurrentAsciiArt() const;
    const std::string& GetCurrentAudio() const;
    bool NeedsAsciiArt() const;
    bool NeedsAudio() const;

    // New method to check if the game is over
    bool IsGameOver() const;

private:
    std::map<std::string, StoryNode> storyNodes;
    std::vector<std::string> randomNodes;
    std::string currentNode;
    InputManager& inputManager;
    RenderManager& renderManager; // Changed to reference
};

#endif // STORY_MANAGER_H
