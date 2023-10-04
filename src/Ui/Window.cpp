#include "Window.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include <cmath>

Window::Window(int screen_height, int screen_width) : SCREEN_HEIGHT{screen_height}, SCREEN_WIDTH{screen_width}, startNewFigure{true}
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw("failed to initialize window");
        return;
    }

    window = SDL_CreateWindow("Polygon Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        throw("failed to create window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        throw("failed to create renderer");
        return;
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::runWindow()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    bool quit = false;
    SDL_Event e;

    drawLine(120, 120, 200, 200);
    SDL_RenderPresent(renderer);


    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                handleMouseClick(e);
            }
        }
    }
}

void Window::runWindowInThread()
{
    std::thread windowThread(&Window::runWindow, this);
    windowThread.join();
}

void Window::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Window::handleMouseClick(SDL_Event e)
{
    if(startNewFigure)
    {
        startNewFigure = false;
        prevX = e.button.x;
        prevY = e.button.y;
        figureStartX = e.button.x;
        figureStartY = e.button.y;
        return;
    }
    if(abs(figureStartX - e.button.x) < Window::epsilon && abs(figureStartY - e.button.y) < Window::epsilon)
    {
        startNewFigure = true;
        drawLine(prevX, prevY, figureStartX, figureStartY);
        SDL_RenderPresent(renderer);
        return;
    }
    drawLine(prevX, prevY, e.button.x, e.button.y);
    prevX = e.button.x;
    prevY = e.button.y;


    SDL_RenderPresent(renderer);
}
