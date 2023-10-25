#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../Logic/Polygon.hpp"
#include <vector>
#include "State.hpp"
#include "../Logic/Point.hpp"


class Window
{
private:
    static const int epsilon = 10;
    static const int pointRadius = 5;

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    
    SDL_Texture* figureTexture;
    SDL_Texture* hullTexture;
    SDL_Texture* drawingTexture;
    SDL_Texture* mouseLineTexture;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    State state;

    void moveMode(SDL_Event e);
    void createMode(SDL_Event e);
    void deleteMode(SDL_Event e);
    void verticalMode(SDL_Event e);
    void horizontalMode(SDL_Event e);
    void hullMode(SDL_Event e);

    void handleMouseClick(SDL_Event e);
    void handleDrag(int x, int y);
    bool handleVertexClick(int x, int y);
    void handleMouseButtonUp();

    void drawPoint(int x, int y, SDL_Texture* texture, int r, int g, int b, int a);
    void drawFigures(std::vector<Polygon> figures, int r, int g, int b, int a, SDL_Texture* texture);
    void drawLine(int x1, int y1, int x2, int y2, SDL_Texture* texture, int r, int g, int b, int a);
    void drawBresenhamLine(int x1, int y1, int x2, int y2, SDL_Texture* texture, int r, int g, int b, int a);
    
    void (Window::*drawLineAlgorithm) (int, int, int, int, SDL_Texture*, int, int, int, int);


    void newFigure(int x, int y);
    void endFigure();
    void addVertex(int x, int y);

    void clearTexture(SDL_Texture* texture);
public:
    Window(int screen_width, int screen_height);
    ~Window();
    void runWindow();
    void runWindowInThread();

};
#endif