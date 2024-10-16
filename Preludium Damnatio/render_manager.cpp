// render_manager.cpp
#include "render_manager.h"
#include <iostream>
#include <fstream>
#include <string>

// Constructor
RenderManager::RenderManager(SDL_Renderer* renderer) : renderer(renderer) {
    if (!renderer) {
        std::cerr << "Failed to initialize RenderManager: Invalid renderer." << std::endl;
    }
}

// Destructor
RenderManager::~RenderManager() {
    // Clean up resources if necessary
}

// Clear the screen
void RenderManager::Clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
    SDL_RenderClear(renderer); // Clear the screen
}

// Present the rendered content
void RenderManager::Present() {
    SDL_RenderPresent(renderer); // Present the rendered content
}

// Render text to console
void RenderManager::RenderText(const std::string& text) {
    std::cout << text << std::endl; // Output text to console
}

// Render ASCII art from a file
void RenderManager::RenderAsciiArt(const std::string& filename) {
    std::ifstream file(filename); // Open the file
    if (!file.is_open()) {
        std::cerr << "Failed to open ASCII art file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl; // Print each line of the ASCII art
    }

    file.close(); // Close the file
}

// Load and render an image
void RenderManager::RenderImage(const std::string& filename, int x, int y, int width, int height) {
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str()); // Load image as surface
    if (!surface) {
        std::cerr << "Failed to load image: " << filename << " SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Create texture from surface
    SDL_FreeSurface(surface); // Free the surface after creating the texture
    if (!texture) {
        std::cerr << "Failed to create texture from surface: SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect dstRect = { x, y, width, height }; // Destination rectangle for rendering
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect); // Render the texture
    SDL_DestroyTexture(texture); // Clean up the texture
}
