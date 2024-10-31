#include "render_manager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

#include <sstream> // Include for std::istringstream

void RenderManager::RenderTextToScreen(const std::string& text, int x, int y, SDL_Color color, int maxWidth) {
    // Check if font is loaded
    if (font == nullptr) {
        std::cerr << "Font not loaded." << std::endl;
        return; // Exit if font is not loaded
    }

    std::istringstream iss(text);
    std::string word;
    std::string line;

    while (iss >> word) {
        // Create a surface for the current line
        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, (line + word + " ").c_str(), color);

        // Check if the line width exceeds maxWidth
        if (lineSurface && lineSurface->w > maxWidth) {
            // Render the current line
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
            SDL_FreeSurface(lineSurface); // Free the surface after creating the texture
            SDL_Rect dstRect = { x, y, lineSurface->w, lineSurface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);

            // Move down for the next line
            y += lineSurface->h;
            line.clear(); // Clear the current line for the next line
        }

        line += word + " "; // Add the word to the current line
    }

    // Render any remaining text in the line
    if (!line.empty()) {
        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line.c_str(), color);
        if (lineSurface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
            SDL_FreeSurface(lineSurface);
            SDL_Rect dstRect = { x, y, lineSurface->w, lineSurface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
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
