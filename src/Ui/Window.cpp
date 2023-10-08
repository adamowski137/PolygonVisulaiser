#include "Window.hpp"
#include "Button.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/backends/imgui_impl_sdl2.h"
#include <GL/glu.h>
#include <iostream>
#include <thread>
#include <cmath>
#include "../Logic/Polygon.hpp"


Window::Window(int screen_height, int screen_width) : SCREEN_HEIGHT{screen_height}, SCREEN_WIDTH{screen_width}, startNewFigure{true}
{
    #ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    SDL_GL_SetAttribute( // required on Mac OS
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
        );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #elif __linux__
        // GL 3.2 Core + GLSL 150
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #elif _WIN32
        // GL 3.0 + GLSL 130
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #endif

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
    TTF_Init();

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL)
    {
        std::cerr << "[ERROR] Failed to create a GL context: "
                  << SDL_GetError() << std::endl;
    }
    SDL_GL_MakeCurrent(window, gl_context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);    
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui::CreateContext();
    ImVec4 background = ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);
    glClearColor(background.x, background.y, background.z, background.w);

    font = TTF_OpenFont("assets/Raleway-Regular.ttf", 32);
    canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    changeModeButton = new Button(renderer, 0, 0, "Change", font);
    state = CREATE;
    leftButtonDown = false;
    selectedPolygon = -1;
}

Window::~Window()
{
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Window::runWindow()
{
    SDL_SetRenderTarget(renderer, canvasTexture);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    bool quit = false;
    SDL_Event e;
    
    changeModeButton->render(renderer);
    while (!quit)
    {
        if (!startNewFigure)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_SetRenderTarget(renderer, nullptr);
            SDL_RenderCopy(renderer, canvasTexture, nullptr, nullptr);
            drawLine(mouseX, mouseY, prevX, prevY);
            SDL_RenderPresent(renderer);
        }

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
            if (e.type == SDL_MOUSEMOTION)
            {
                handleDrag(e.motion.x, e.motion.y);
            }
                
            if (e.type == SDL_MOUSEBUTTONUP)
            {
                leftButtonDown = false;
            }
        }
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
}

void Window::handleDrag(int x, int y)
{
    static const int inputThrottleMs = 16; // Adjust as needed
    static Uint32 lastDragTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (leftButtonDown && selectedPolygon != -1 && (currentTime - lastDragTime) >= inputThrottleMs)
    {
        lastDragTime = currentTime;
        
        Polygon polygon = figures[selectedPolygon];
        polygon.move(x - offsetX, y - offsetY);
        figures[selectedPolygon] = polygon;
        drawFigures();
        offsetX = x;
        offsetY = y;
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
    int x = e.button.x;
    int y = e.button.y;


    if(changeModeButton->isButtonClicked(x, y))
    {
        if(state == CREATE)
            state = MOVE;
        else if(state == MOVE)
            state = CREATE;
        return;
    }
    if(state == MOVE)
    {
        moveMode(x, y);
    }

    if(state == CREATE)
    {
        createMode(x, y);
    }   
}

void Window::moveMode(int x, int y)
{
    if(leftButtonDown == false)
    {
        for(int i = 0; i < figures.size(); i++) 
        {
            if(figures[i].isPointInside(x, y))
            {
                selectedPolygon = i;
                leftButtonDown = true;
                offsetX = x;
                offsetY = y;
                return;
            }
        }

        selectedPolygon = -1;
    }
}

void Window::createMode(int x, int y)
{
    SDL_SetRenderTarget(renderer, canvasTexture);

    if(startNewFigure)
    {
        newFigure(x, y);
        return;
    }

    if(abs(figureStartX - x) < Window::epsilon && abs(figureStartY - y) < Window::epsilon)
    {
        endFigure();
        return;
    }

    addVertex(x, y);

    return;

}

void Window::drawPoint(int x, int y)
{
    SDL_Rect rect;
    rect.x = x - Window::pointRadius;
    rect.y = y - Window::pointRadius;
    rect.w = 2 * Window::pointRadius;
    rect.h = 2 * Window::pointRadius;
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Window::newFigure(int x, int y)
{
    current = Polygon();
    current.addVertex(x, y);
    startNewFigure = false;
    prevX = x;
    prevY = y;
    figureStartX = x;
    figureStartY = y;
    drawPoint(x, y);
    SDL_RenderPresent(renderer);
}

void Window::endFigure()
{
    drawLine(prevX, prevY, figureStartX, figureStartY);
    figures.push_back(current);
    startNewFigure = true;
    drawFigures();
}

void Window::addVertex(int x, int y)
{
    drawPoint(x, y);
    drawLine(prevX, prevY, x, y);
    prevX = x;
    prevY = y;
    current.addVertex(x, y);
    SDL_RenderPresent(renderer);
}

void Window::drawFigures()
{
    SDL_SetRenderTarget(renderer, canvasTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for(auto polygon : figures)
    {
        int count = polygon.getCount();
        short int x[count];
        short int y[count];
        polygon.getXCordinates(x);
        polygon.getYCordinates(y);
        filledPolygonRGBA(renderer, x, y, count, 255, 0, 0, 150);
        drawPoint(x[0], y[0]);
        drawLine(x[0], y[0], x[count - 1], y[count - 1]);
        for(int i = 1; i < count; i++)
        {
            drawLine(x[i - 1], y[i - 1], x[i], y[i]);
            drawPoint(x[i], y[i]);
        }
    }
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, canvasTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

}