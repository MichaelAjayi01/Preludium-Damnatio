#include "story_manager.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

StoryManager::StoryManager(InputManager& inputManager, RenderManager& renderManager)
    : currentNode("start"),
    inputManager(inputManager),
    renderManager(renderManager)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}



void StoryManager::LoadStory() {
    // Key story nodes (fixed plot points)
    storyNodes["start"] = StoryNode(
        "\"Where...am I?\"",
        { "Proceed" },
        { {0, "selection_menu"} }
    );

    storyNodes["selection_menu"] = StoryNode(
        "You stand before three ancient doors, each carved with strange, foreboding symbols...",
        { "Enter the stone door", "Enter the golden door", "Enter the iron door" },
        { {0, "stone_room"}, {1, "golden_room"}, {2, "iron_room"} },
        "assets/story node images/ascii art doors.bmp"
    );

    // Story nodes for different room selections
    storyNodes["stone_room"] = StoryNode(
        "The stone door creaks open, revealing a chamber lined with old tombs. An eerie silence envelops you. As you step in, you hear whispers echoing off the walls.",
        { "Search for hidden treasures", "Leave the room" },
        { {0, "find_treasure"}, {1, "selection_menu"} }
    );

    storyNodes["golden_room"] = StoryNode(
        "You enter a dazzling room filled with golden artifacts and shimmering jewels. But there’s a sense of danger that hangs in the air like a thick fog.",
        { "Take a jewel", "Investigate the room", "Leave the room" },
        { {0, "curse_jewel"}, {1, "golden_secrets"}, {2, "selection_menu"} }
    );

    storyNodes["iron_room"] = StoryNode(
        "A cold breeze greets you as you step into the iron room. A large iron gate stands at the end, slightly ajar, emitting an ominous glow.",
        { "Push open the gate", "Examine the room for secrets", "Leave the room" },
        { {0, "necromancer_lair"}, {1, "iron_secrets"}, {2, "selection_menu"} }
    );

    // Treasure and secrets nodes
    storyNodes["find_treasure"] = StoryNode(
        "You uncover a hidden chest filled with ancient relics. One item stands out: a necromancer’s crown. As you approach, shadows swirl around the chest.",
        { "Take the crown", "Leave it behind", "Inspect the shadows" },
        { {0, "crown_choice"}, {1, "selection_menu"}, {2, "shadow_interaction"} }
    );

    storyNodes["shadow_interaction"] = StoryNode(
        "You reach out to touch the swirling shadows. They pull you in, revealing a vision of the necromancer's past—his rise to power and subsequent fall.",
        { "Embrace the vision", "Break free from it" },
        { {0, "vision_choice"}, {1, "selection_menu"} }
    );

    storyNodes["vision_choice"] = StoryNode(
        "The vision overwhelms you, and you feel your consciousness merging with the necromancer's. You gain knowledge of dark spells.",
        { "Use the spells", "Resist the knowledge" },
        { {0, "corrupted"}, {1, "selection_menu"} }
    );

    storyNodes["golden_secrets"] = StoryNode(
        "In the corner, you spot an ancient tome, its pages flickering with a strange light. It seems to call to you.",
        { "Read the tome", "Take a jewel", "Leave the room" },
        { {0, "tome_choice"}, {1, "curse_jewel"}, {2, "selection_menu"} }
    );

    storyNodes["tome_choice"] = StoryNode(
        "As you read the tome, you learn about forbidden magic that can grant immense power. But with power comes a price.",
        { "Accept the knowledge", "Close the book and leave" },
        { {0, "corrupted"}, {1, "selection_menu"} }
    );

    storyNodes["curse_jewel"] = StoryNode(
        "As you grasp the jewel, a dark energy envelops you. You feel your life force wane. A sinister voice whispers promises of power in exchange for your soul.",
        { "Embrace the dark power", "Try to resist it", "Throw the jewel away" },
        { {0, "necromancer_lair"}, {1, "selection_menu"}, {2, "selection_menu"} }
    );

    storyNodes["iron_secrets"] = StoryNode(
        "You discover a hidden alcove containing scrolls of dark magic. One scroll hints at the necromancer’s fate, warning of the consequences of greed.",
        { "Read the scroll", "Leave the scroll", "Burn the scroll" },
        { {0, "necromancer_lair"}, {1, "selection_menu"}, {2, "selection_menu"} }
    );

    // Necromancer lair nodes
    storyNodes["necromancer_lair"] = StoryNode(
        "You step into the lair of the necromancer, surrounded by dark energy and remnants of his power. Shadows writhe in anticipation of your presence.",
        { "Search for clues", "Examine the dark altar", "Leave the lair" },
        { {0, "main_necromancer_lair"}, {1, "dark_altar"}, {2, "selection_menu"} }
    );

    storyNodes["main_necromancer_lair"] = StoryNode(
        "You feel a presence watching you. A whisper fills the air, urging you to take the crown. The atmosphere thickens, making it hard to breathe.",
        { "Take the crown", "Refuse the crown", "Call out to the presence" },
        { {0, "crown_choice"}, {1, "grave_choice"}, {2, "whisper_choice"} }
    );

    storyNodes["whisper_choice"] = StoryNode(
        "You call out, demanding to know who watches you. A shadowy figure appears, offering you a deal—a chance to become the next necromancer.",
        { "Accept the offer", "Decline and fight" },
        { {0, "corrupted"}, {1, "grave_choice"} }
    );

    storyNodes["dark_altar"] = StoryNode(
        "The altar pulses with dark energy. You see the crown resting atop it, glowing ominously. The air feels charged with power, tempting you.",
        { "Take the crown", "Leave it alone", "Perform a ritual" },
        { {0, "crown_choice"}, {1, "selection_menu"}, {2, "dark_ritual"} }
    );

    storyNodes["dark_ritual"] = StoryNode(
        "You decide to perform a dark ritual, channeling the energy from the altar. The air crackles as you summon shadows to aid you.",
        { "Command the shadows", "Break the ritual" },
        { {0, "corrupted"}, {1, "selection_menu"} }
    );

    // Crown choice nodes
    storyNodes["crown_choice"] = StoryNode(
        "You place the necromancer’s crown upon your head. A rush of dark power surges through you, reshaping your very essence.",
        { "Embrace the corruption", "Resist the power", "Dismantle the crown" },
        { {0, "corrupted"}, {1, "grave_choice"}, {2, "selection_menu"} }
    );

    storyNodes["grave_choice"] = StoryNode(
        "You feel your life force slowly draining into the necromancer's grave, transferring your essence into his. Shadows beckon you deeper into the grave.",
        { "Surrender to the drain", "Fight against it", "Seek a way out" },
        { {0, "sacrificed"}, {1, "freed"}, {2, "selection_menu"} }
    );

    // End nodes
    storyNodes["corrupted"] = StoryNode(
        "Corruption seeps into your soul, and you become the new necromancer, bound to darkness forever. Your eyes glow with malevolence, and you lose your humanity.",
        { "GAME OVER" },
        {}
    );

    storyNodes["sacrificed"] = StoryNode(
        "Your life force becomes one with the necromancer, granting him new power while you fade into oblivion. His laughter echoes in your mind as your essence is consumed.",
        { "GAME OVER" },
        {}
    );

    storyNodes["freed"] = StoryNode(
        "You break free from the necromancer's influence, escaping with your life, but forever haunted by the dark choices you made. You emerge into the light, but darkness lingers at the edges of your mind.",
        { "GAME OVER" },
        {}
    );
}



void StoryManager::DisplayCurrentNode() {
    const StoryNode& node = storyNodes[currentNode];
    std::cout << "Displaying Current Node: " << currentNode << std::endl;
    std::cout << "Node Text: " << node.text << std::endl;

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    const int maxWidth = 600;
    int nodeTextHeight = 0;

    renderManager.RenderTextToScreen(node.text, 10, 10, textColor, maxWidth, &nodeTextHeight);
    int imageStartY = 10 + nodeTextHeight + 20;

    if (!node.imageFile.empty()) {
        int imageWidth = 750;
        int imageHeight = 750;
        renderManager.RenderImage(node.imageFile, 10, imageStartY, imageWidth, imageHeight);
        imageStartY += imageHeight + 20;
    }
    else {
        imageStartY += 20;
    }

    int optionsStartY = imageStartY;
    for (size_t i = 0; i < node.options.size(); ++i) {
        std::string optionText = std::to_string(i + 1) + ": " + node.options[i];
        std::cout << "Option " << i + 1 << ": " << optionText << std::endl;
        renderManager.RenderTextToScreen(optionText, 10, optionsStartY, textColor, maxWidth);
        optionsStartY += 30;
    }
}

void StoryManager::HandleChoice(int choice) {
    if (choice < 1 || choice > storyNodes[currentNode].options.size()) {
        throw std::out_of_range("Choice out of range");
    }

    // Move to the next node based on player's choice
    currentNode = storyNodes[currentNode].nextNodes[choice - 1].second;
    DisplayCurrentNode();
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
