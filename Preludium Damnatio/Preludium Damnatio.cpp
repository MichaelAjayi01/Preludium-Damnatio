// main.cpp
#include "story_manager.h"
#include "input_manager.h"
#include "render_manager.h"
#include "audio_manager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#define SDL_MAIN_HANDLED

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1; // Exit if initialization fails
    }

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("Preludium Damnatio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1; // Exit if window creation fails
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1; // Exit if renderer creation fails
    }

    // Initialize managers
// Initialize managers
    InputManager inputManager;
    StoryManager storyManager(inputManager); 
    RenderManager renderManager(renderer);
    AudioManager audioManager;


    storyManager.LoadStory();

	storyManager.DisplayCurrentNode();

    // Game loop
    while (true) {
        int choice = inputManager.GetPlayerChoice(storyManager.GetCurrentOptions().size());
        storyManager.HandleChoice(choice);

        // Play audio and render art if necessary
        if (storyManager.NeedsAsciiArt()) {
            renderManager.RenderAsciiArt(storyManager.GetCurrentAsciiArt());
        }
        if (storyManager.NeedsAudio()) {
            audioManager.PlayAudio(storyManager.GetCurrentAudio());
        }
    }

    // Clean up and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
