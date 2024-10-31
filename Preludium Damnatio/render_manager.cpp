#include "render_manager.h"
#include <iostream>
#include <fstream>
#include <string>

// Constructor
RenderManager::RenderManager(SDL_Renderer* renderer) : renderer(renderer), font(nullptr), initialized(renderer != nullptr) {
    if (!renderer) {
        std::cerr << "Failed to initialize RenderManager: Invalid renderer." << std::endl;
    }
}

// Destructor
RenderManager::~RenderManager() {
    if (font) {
        TTF_CloseFont(font);
    }
}

// Check if RenderManager is initialized
bool RenderManager::IsInitialized() const {
    return initialized; // Return the initialization status
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

// Load and set the font
bool RenderManager::LoadFont(const std::string& fontPath, int fontSize) {
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "Failed to load font at " << fontPath << ": " << TTF_GetError() << std::endl;
    }
    return font != nullptr;
}

void RenderManager::RenderTextToScreen(const std::string& text, int x, int y, SDL_Color color) {
    // Check if font is loaded
    if (font == nullptr) {
        std::cerr << "Font not loaded." << std::endl;
        return; // Exit if font is not loaded
    }

    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return; // Exit if text surface creation fails
    }

    // Set up the destination rectangle for rendering the texture
    SDL_Rect dstRect = { x, y, textSurface->w, textSurface->h };

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Free the surface after creating the texture
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return; // Exit if texture creation fails
    }

    // Render the texture to the screen
    if (SDL_RenderCopy(renderer, texture, nullptr, &dstRect) < 0) {
        std::cerr << "Failed to render texture: " << SDL_GetError() << std::endl;
    }

    // Clean up the texture
    SDL_DestroyTexture(texture);
}


// Render ASCII art to the SDL window
void RenderManager::RenderAsciiArtToScreen(const std::string& asciiArt, int startX, int startY, SDL_Color color) {
    int x = startX;
    int y = startY;

    for (char c : asciiArt) {
        if (c == '\n') {
            // Move to the next line
            x = startX;
            y += TTF_FontHeight(font); // Move down by one character height
        }
        else {
            // Render each character as a texture
            std::string character(1, c); // Create a string from the character
            RenderTextToScreen(character, x, y, color);

            // Get the width of the rendered character
            int charWidth;
            TTF_SizeText(font, character.c_str(), &charWidth, nullptr); // Get character width

            x += charWidth; // Move right by the width of the character
        }
    }
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
