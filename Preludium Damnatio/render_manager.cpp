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

TTF_Font* RenderManager::GetFont() const {
    return font; // Return the font member variable
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

bool RenderManager::LoadFont(const std::string& fontPath, int fontSize) {
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "Failed to load font at " << fontPath << ": " << TTF_GetError() << std::endl;
        return false; // Return false if the font couldn't be loaded
    }
    return true; // Return true if the font loaded successfully
}



void RenderManager::RenderTextToScreen(const std::string& text, int x, int y, SDL_Color color, int maxWidth, int* totalHeight) {
    if (font == nullptr) {
        std::cerr << "Font not loaded." << std::endl;
        return; // Exit if font is not loaded
    }

    std::istringstream iss(text);
    std::string word;
    std::string line;
    int initialY = y;
    int lineHeight = TTF_FontHeight(font);

    while (iss >> word) {
        // Check if adding the word would exceed maxWidth
        std::string newLine = line.empty() ? word : line + " " + word;
        int newLineWidth;
        TTF_SizeText(font, newLine.c_str(), &newLineWidth, nullptr);

        if (newLineWidth > maxWidth && !line.empty()) {
            // Render current line as texture if it would exceed maxWidth
            SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line.c_str(), color);
            if (lineSurface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
                SDL_Rect dstRect = { x, y, lineSurface->w, lineSurface->h };
                SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(lineSurface);
            }

            y += lineHeight; // Move down to start a new line
            line = word; // Start a new line with the current word
        }
        else {
            line = newLine; // Append word to the current line
        }
    }

    // Render any remaining text in the line buffer
    if (!line.empty()) {
        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line.c_str(), color);
        if (lineSurface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
            SDL_Rect dstRect = { x, y, lineSurface->w, lineSurface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(lineSurface);
        }
        y += lineHeight; // Account for the last rendered line
    }

    // Set total height if a pointer is passed
    if (totalHeight) {
        *totalHeight = y - initialY; // Calculate the height of all rendered text
    }
}


void RenderManager::RenderImage(const std::string& filename, int x, int y, int width, int height) {
    std::cout << "Attempting to load image: " << filename << std::endl; // Debug statement

    // Load image as surface
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << filename << " SDL_Error: " << SDL_GetError() << std::endl;
        return; // Early exit if the image fails to load
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface after creating the texture
    if (!texture) {
        std::cerr << "Failed to create texture from surface: SDL_Error: " << SDL_GetError() << std::endl;
        return; // Early exit if texture creation fails
    }

    // Debug output for rendering
    std::cout << "Rendering image: " << filename
        << " at position (" << x << ", " << y << ") "
        << "with size (" << width << " x " << height << ")" << std::endl;

    SDL_Rect dstRect = { x, y, width, height }; // Destination rectangle for rendering
    if (SDL_RenderCopy(renderer, texture, nullptr, &dstRect) != 0) {
        std::cerr << "Failed to render texture: SDL_Error: " << SDL_GetError() << std::endl;
    }

    SDL_DestroyTexture(texture); // Clean up the texture
}


