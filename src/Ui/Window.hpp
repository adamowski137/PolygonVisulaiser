#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_


#include <SDL2/SDL.h>

class Window
{
private:
    static const int epsilon = 10;
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    SDL_Window* window;
    SDL_Renderer* renderer;

    void drawLine(int x1, int y1, int x2, int y2);
    void handleMouseClick(SDL_Event e);

    bool startNewFigure;
    int figureStartX;
    int figureStartY;
    int prevX;
    int prevY;
public:
    Window(int screen_width, int screen_height);
    ~Window();
    void runWindow();
    void runWindowInThread();

};
#endif