#include "Window.hpp"
#include "SDL.h"
#include <iostream>
#include <thread>

Window::Window(int screen_height, int screen_width) : SCREEN_HEIGHT{screen_height}, SCREEN_WIDTH{screen_width} 
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw("failed to initialize window");
        return;
    }
    
    window = SDL_CreateWindow("Polygon Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        throw("failed to create window");
        return;

    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
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
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
}

void Window::runWindowInThread()
{
    std::thread windowThread(&Window::runWindow, this);
    windowThread.join();
}