// render_manager.h

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <string>
#include <SDL.h>

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

    // Render text to console
    void RenderText(const std::string& text); // Add this line

    // Render ASCII art from a file
    void RenderAsciiArt(const std::string& filename); // Corrected parameter type

    // Load and render an image
    void RenderImage(const std::string& filename, int x, int y, int width, int height);

private:
    SDL_Renderer* renderer; // Pointer to the SDL renderer
};

#endif // RENDER_MANAGER_H
