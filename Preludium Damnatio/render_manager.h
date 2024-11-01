#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class RenderManager {
public:
    // Constructor
    RenderManager(SDL_Renderer* renderer);

    // Destructor
    ~RenderManager();

    // Clear the screen
    void Clear();

    // Present the rendered content
    void Present();

    // Load and set the font
    bool LoadFont(const std::string& fontPath, int fontSize);

    TTF_Font* GetFont() const;

    // Check if RenderManager is initialized
    bool IsInitialized() const;

    // Render text to SDL window with optional width for wrapping and total height calculation
    void RenderTextToScreen(const std::string& text, int x, int y, SDL_Color color = { 255, 255, 255, 255 }, int maxWidth = 780, int* totalHeight = nullptr);

    // Render ASCII art to the SDL window
    void RenderAsciiArtToScreen(const std::string& asciiArt, int startX, int startY, SDL_Color color = { 255, 255, 255, 255 });

    // Load and render an image
    void RenderImage(const std::string& filename, int x, int y, int width, int height);

private:
    SDL_Renderer* renderer; // Pointer to the SDL renderer
    TTF_Font* font; // Pointer to the loaded font
    bool initialized; // Flag to check if RenderManager is initialized
};

#endif
