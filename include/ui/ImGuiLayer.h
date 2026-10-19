#pragma once

#include <SDL2/SDL.h>  
#include <SDL_opengl.h>

class ImGuiLayer
{
public:
    // Initialize SDL, OpenGL, and ImGui
    bool init();

    // Begin a new ImGui frame (call once per loop)
    void beginFrame();

    // Render ImGui and swap buffers (call once per loop)
    void endFrame();

    // Shutdown ImGui, OpenGL context, and SDL
    void shutdown();

    // Access to the SDL window if needed
    SDL_Window* getWindow() const { return window; }

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
};
