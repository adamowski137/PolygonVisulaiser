#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_


#include "SDL.h"

class Window
{
private:
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Window(int screen_width, int screen_height);
    ~Window();
    void runWindow();
    void runWindowInThread();
};
#endif