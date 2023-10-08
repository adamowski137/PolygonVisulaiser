#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Button.hpp"
#include "../Logic/Polygon.hpp"
#include <vector>
#include "State.hpp"


class Window
{
private:
    static const int epsilon = 10;
    static const int pointRadius = 5;

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    
    SDL_Texture* canvasTexture;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Button* changeModeButton;

    State state;

    std::vector<Polygon> figures;
    Polygon current;

    bool startNewFigure;
    int figureStartX;
    int figureStartY;
    int prevX;
    int prevY;
    bool leftButtonDown;
    int selectedPolygon;
    int offsetX;
    int offsetY;

    void moveMode(int x, int y);
    void createMode(int x, int y);

    void handleMouseClick(SDL_Event e);
    void handleDrag(int x, int y);

    void drawPoint(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawFigures();
    
    void newFigure(int x, int y);
    void endFigure();
    void addVertex(int x, int y);
public:
    Window(int screen_width, int screen_height);
    ~Window();
    void runWindow();
    void runWindowInThread();

};
#endif