#define _CRT_SECURE_NO_WARNINGS
#include "story_manager.h"
#include "input_manager.h"
#include "render_manager.h"
#include "audio_manager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <direct.h>  // Include this for _getcwd
#include <cstring>

#define SDL_MAIN_HANDLED

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    std::cout << "SDL initialized successfully." << std::endl;

    // Get and print the current working directory
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }
    else {
        std::cerr << "Error getting current working directory." << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    std::cout << "SDL_ttf initialized successfully." << std::endl;

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("Preludium Damnatio",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1300, 1000,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    std::cout << "Window created successfully." << std::endl;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    std::cout << "Renderer created successfully." << std::endl;

    // Initialize managers
    InputManager inputManager;
    RenderManager renderManager(renderer);
    StoryManager storyManager(inputManager, renderManager);
    AudioManager audioManager;

    if (!renderManager.IsInitialized()) {
        std::cerr << "RenderManager failed to initialize." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    else {
        std::cout << "RenderManager initialized with a valid renderer." << std::endl;
    }

    // Load the story
    storyManager.LoadStory();

    // Construct the path to the font file
    std::string fontPath = std::string(cwd) + "\\assets\\fonts\\BonaNovaSC-Regular.ttf";
    std::cout << "Attempting to load font from: " << fontPath << std::endl;

    // Check if the font file can be opened
    FILE* file;
    if (fopen_s(&file, fontPath.c_str(), "r") == 0) {
        std::cout << "Font file is accessible." << std::endl;
        fclose(file);
    }

    if (!renderManager.LoadFont(fontPath.c_str(), 18)) {
        std::cerr << "Failed to load font." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    std::cout << "Font loaded successfully." << std::endl;

    // Display the initial story node once after loading the story
    renderManager.Clear();              // Clear screen for fresh render
    storyManager.DisplayCurrentNode();   // Display the first story node
    renderManager.Present();             // Present to ensure it's visible
    std::cout << "Story loaded successfully. Starting game loop." << std::endl;

    // Set focus to the SDL window
    SDL_RaiseWindow(window); // Brings the SDL window to the front
    SDL_SetWindowFullscreen(window, 0); // Optionally remove fullscreen if previously set

    // Game loop
    while (true) {
        SDL_Event e; // Create an SDL event variable

        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                // Handle the quit event
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                TTF_Quit();
                SDL_Quit();
                std::cout << "Cleaned up and exited." << std::endl;
                return 0;
            }
        }

        renderManager.Clear();  // Clear screen at the start of each loop

        int choice = inputManager.GetPlayerChoice(storyManager.GetCurrentOptions().size());
        storyManager.HandleChoice(choice);

        // Display the current node based on the choice made
        storyManager.DisplayCurrentNode();

        // Render ASCII art if needed
        if (storyManager.NeedsAsciiArt()) {
            renderManager.RenderAsciiArtToScreen(storyManager.GetCurrentAsciiArt(), 100, 100);
            std::cout << "Rendered ASCII art to screen." << std::endl;
        }

        // Play audio if needed
        if (storyManager.NeedsAudio()) {
            audioManager.PlayAudio(storyManager.GetCurrentAudio());
            std::cout << "Played audio." << std::endl;
        }

        renderManager.Present(); // Present rendered content to the screen
        std::cout << "Presented content to the screen." << std::endl;
    }

    // Clean up and quit (this won't be reached due to the infinite loop)
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Cleaned up and exited." << std::endl;

    return 0;
}
