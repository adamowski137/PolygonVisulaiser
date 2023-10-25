#include "Window.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/backends/imgui_impl_sdl2.h"
#include "../../imgui/backends/imgui_impl_sdlrenderer2.h"
#include <GL/glu.h>
#include <iostream>
#include <thread>
#include <cmath>
#include "../Logic/Polygon.hpp"

Window::Window(int screen_width, int screen_height) : SCREEN_HEIGHT{screen_height}, SCREEN_WIDTH{screen_width}
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


    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    figureTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    hullTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    drawingTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    mouseLineTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    clearTexture(figureTexture);
    clearTexture(hullTexture);
    clearTexture(drawingTexture);
    clearTexture(mouseLineTexture);
    SDL_SetTextureBlendMode(figureTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(hullTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(drawingTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mouseLineTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    state = State{};
    drawLineAlgorithm = &Window::drawLine;
}

Window::~Window()
{
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyTexture(figureTexture);
    SDL_DestroyTexture(hullTexture);
    SDL_DestroyTexture(drawingTexture);
    SDL_DestroyTexture(mouseLineTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Window::runWindow()
{

    bool quit = false;
    SDL_Event e;

    int value = 0;

    while (!quit)
    {
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Options");
        if (ImGui::Button("Create Mode", ImVec2(200, 30)))
        {
            state.applicationState = CREATE;
        }
        if (ImGui::Button("Edit Mode", ImVec2(200, 30)))
        {
            state.applicationState = MOVE;
        }
        if (ImGui::Button("Delete Mode", ImVec2(200, 30)))
        {
            state.applicationState = DELETE;
        }
        if (ImGui::Button("Set Vetical Line", ImVec2(200, 30)))
        {
            state.applicationState = VERTICAL;
        }
        if (ImGui::Button("Set Horizontal Line", ImVec2(200, 30)))
        {
            state.applicationState = HORIZONTAL;
        }
        if (ImGui::Button("Hull", ImVec2(200, 30)))
        {
            state.currentHull = nullptr;
            state.applicationState = HULL;
            state.hull = !state.hull;
            clearTexture(hullTexture);
        }
        static int selected = 0;
        ImGui::RadioButton("library algorithm", &selected, 0); ImGui::SameLine();
        ImGui::RadioButton("Brensenham algorithm", &selected, 1);

        if(selected == 0)
        {
            drawLineAlgorithm = &Window::drawLine;
        }
        else if(selected == 1)
        {
            drawLineAlgorithm = &Window::drawBresenhamLine;
        }
        ImGui::SliderInt("Offset", &value, 0, 100);
        if(state.currentHull != nullptr)
        {
            *(state.currentHull) = value;
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        
        while (SDL_PollEvent(&e) != 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && !ImGui::GetIO().WantCaptureMouse)
            {
                handleMouseClick(e);
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                handleDrag(e.motion.x, e.motion.y);
            }

            if (e.type == SDL_MOUSEBUTTONUP)
            {
                handleMouseButtonUp();
            }
        }
        if (!state.startNewFigure)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            (this->*drawLineAlgorithm)(mouseX, mouseY, state.prevX, state.prevY, mouseLineTexture,255, 100, 100, 255);
        }
        clearTexture(figureTexture);
        clearTexture(hullTexture);
        if(state.hull)
        {
            std::vector<Polygon> polygons;
            for(int i = 0; i < state.figures.size(); i++)
            {
                polygons.push_back(state.figures[i].getShiftedPolygon(state.figures[i].hullOffset));
            }
            drawFigures(polygons, 0, 0, 255, 50, hullTexture);
        }
        drawFigures(state.figures, 255, 0, 0, 100, figureTexture);
        state.figuresChanged = false;
        SDL_RenderCopy(renderer, hullTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, drawingTexture, nullptr, nullptr);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, figureTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, mouseLineTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        clearTexture(mouseLineTexture);
        clearTexture(figureTexture);
        clearTexture(hullTexture);
    }

}

void Window::handleDrag(int x, int y)
{
    static const int inputThrottleMs = 16;
    static Uint32 lastDragTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (state.leftButtonDown && state.draggedVertex != nullptr && (currentTime - lastDragTime) >= inputThrottleMs)
    {
        state.figuresChanged = true;
        lastDragTime = currentTime;
        state.draggedVertex->setX(x);
        state.draggedVertex->setY(y);
        state.draggedPolygon->updateHorizontalAndVertical(state.draggedVertex);

        return;
    }

    if(state.leftButtonDown && state.draggedEdge != nullptr && (currentTime - lastDragTime) >= inputThrottleMs)
    {
        state.figuresChanged = true;
        lastDragTime = currentTime;
        int x1 = state.draggedEdge->first->getX();
        int y1 = state.draggedEdge->first->getY();
        int x2 = state.draggedEdge->second->getX();
        int y2 = state.draggedEdge->second->getY();

        state.draggedEdge->first->setX(x1 + x - state.offsetX);
        state.draggedEdge->first->setY(y1 + y - state.offsetY);
        state.draggedEdge->second->setX(x2 + x - state.offsetX);
        state.draggedEdge->second->setY(y2 + y - state.offsetY);

        state.draggedPolygon->updateHorizontalAndVertical(state.draggedEdge->first);
        state.draggedPolygon->updateHorizontalAndVertical(state.draggedEdge->second);

        state.offsetX = x;
        state.offsetY = y;
        return;
    }

    if (state.leftButtonDown && state.draggedPolygon != nullptr && (currentTime - lastDragTime) >= inputThrottleMs)
    {
        state.figuresChanged = true;
        lastDragTime = currentTime;
        state.draggedPolygon->move(x - state.offsetX, y - state.offsetY);
        state.offsetX = x;
        state.offsetY = y;
        return;
    }
}

void Window::runWindowInThread()
{
    std::thread windowThread(&Window::runWindow, this);
    windowThread.join();
}


void Window::handleMouseClick(SDL_Event e)
{

    if(state.applicationState == MOVE)
    {
        moveMode(e);
    }
    if(state.applicationState == CREATE)
    {
        createMode(e);
    }
    if(state.applicationState == DELETE)
    {
        deleteMode(e);
    }
    if(state.applicationState == VERTICAL)
    {
        verticalMode(e);
    }
    if(state.applicationState == HORIZONTAL)
    {
        horizontalMode(e);
    }
    if(state.applicationState == HULL)
    {
        hullMode(e);
    }
}

void Window::moveMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    if(e.button.button == SDL_BUTTON_RIGHT)
    {
        for(auto& figure : state.figures) 
        {
            state.draggedEdge = figure.getClickedEdge(x, y, epsilon * 3);
            if (state.draggedEdge != nullptr)
            {
                state.figuresChanged = true;
                figure.insertOnEdge(state.draggedEdge->first, state.draggedEdge->second);
                return;
            }
        }
    }
    if(state.leftButtonDown == false && e.button.button == SDL_BUTTON_LEFT)
    {
        if (handleVertexClick(x, y))
        {
            state.figuresChanged = true;
            state.leftButtonDown = true;
            state.offsetX = x;
            state.offsetY = y;
            return;
        }
        for(auto& figure : state.figures) 
        {
            state.draggedEdge = figure.getClickedEdge(x, y, epsilon* 3);
            if (state.draggedEdge != nullptr)
            {
                state.figuresChanged = true;
                state.draggedPolygon = &figure;
                state.leftButtonDown = true;
                state.offsetX = x;
                state.offsetY = y;
                return;
            }
            if (figure.isPointInside(x, y))
            {
                state.figuresChanged = true;
                state.hullEditPolygon = &figure;
                state.draggedPolygon = &figure;
                state.leftButtonDown = true;
                state.offsetX = x;
                state.offsetY = y;
                return;
            }
        }
        state.hullEditPolygon = nullptr;
        state.draggedPolygon = nullptr;
    }
}

void Window::deleteMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    if(handleVertexClick(x, y) && state.draggedVertex != nullptr)
    {
        for(int i = 0; i < state.figures.size(); i++)
        {
            if(state.figures[i].deleteVertex(*state.draggedVertex) && state.figures[i].getCount() == 0)
            {
                state.figuresChanged = true;
                state.figures.erase(state.figures.begin() + i);
                return;
            }
        }
        return;
    }
    int i = 0;
    for(auto& figure : state.figures) 
    {
        if (figure.isPointInside(x, y))
        {
            state.figuresChanged = true;
            state.figures.erase(state.figures.begin() + i);
            return;
        }
        i++;
    }
    
}

void Window::hullMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    
    for(auto& figure : state.figures)
    {
        if(figure.isPointInside(x,y))
        {
            state.currentHull = &(figure.hullOffset);
            return;
        }
    }
    state.currentHull = nullptr;
}

void Window::createMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    if(state.startNewFigure)
    {
        newFigure(x, y);
        return;
    }

    if(abs(state.figureStartX - x) < Window::epsilon && abs(state.figureStartY - y) < Window::epsilon)
    {
        endFigure();
        return;
    }

    addVertex(x, y);
}

void Window::verticalMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    for(auto& polygon : state.figures)
    {
        state.draggedEdge = polygon.getClickedEdge(x, y, epsilon * 3);
        if(state.draggedEdge != nullptr)
        {
            polygon.setVertical(state.draggedEdge->first, state.draggedEdge->second);
            return;
        }
    }    
}
void Window::horizontalMode(SDL_Event e)
{
    int x = e.button.x;
    int y = e.button.y;
    for(auto& polygon : state.figures)
    {
        state.draggedEdge = polygon.getClickedEdge(x, y, epsilon * 3);
        if(state.draggedEdge != nullptr)
        {
            polygon.setHorizontal(state.draggedEdge->first, state.draggedEdge->second);
            return;
        }
    }    
}

void Window::newFigure(int x, int y)
{
    state.current = Polygon();
    state.current.addVertex(x, y);
    state.startNewFigure = false;
    state.prevX = x;
    state.prevY = y;
    state.figureStartX = x;
    state.figureStartY = y;
    drawPoint(x, y, drawingTexture, 255, 100, 100, 255);
}

void Window::endFigure()
{
    state.current.borderColor[0] = 255;
    state.current.borderColor[1] = 100;
    state.current.borderColor[2] = 100;
    state.current.borderColor[3] = 255;


    state.figures.push_back(state.current);
    state.startNewFigure = true;
    clearTexture(drawingTexture);
}

void Window::addVertex(int x, int y)
{
    drawPoint(x, y, drawingTexture, 255, 100, 100, 255);
    (this->*drawLineAlgorithm)(state.prevX, state.prevY, x, y, drawingTexture, 255, 100, 100, 255);
    state.prevX = x;
    state.prevY = y;
    state.current.addVertex(x, y);
}

void Window::drawPoint(int x, int y, SDL_Texture* texture, int r, int g, int b, int a)
{
    SDL_Rect rect;
    rect.x = x - Window::pointRadius;
    rect.y = y - Window::pointRadius;
    rect.w = 2 * Window::pointRadius;
    rect.h = 2 * Window::pointRadius;
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}
void Window::drawLine(int x1, int y1, int x2, int y2, SDL_Texture* texture, int r, int g, int b, int a)
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void Window::drawFigures(std::vector<Polygon> polygons, int r, int g, int b, int a, SDL_Texture* texture)
{
    SDL_SetRenderTarget(renderer, texture);
    for(auto& polygon : polygons)
    {
        int count = polygon.getCount();
        short int x[count];
        short int y[count];
        polygon.getXCordinates(x);
        polygon.getYCordinates(y);
        filledPolygonRGBA(renderer, x, y, count, r, g, b, a);
        drawPoint(x[0], y[0], texture, polygon.borderColor[0], polygon.borderColor[1], polygon.borderColor[2], polygon.borderColor[3]);
        (this->*drawLineAlgorithm)(x[0], y[0], x[count - 1], y[count - 1], texture, polygon.borderColor[0], polygon.borderColor[1], polygon.borderColor[2], polygon.borderColor[3]);
        for(int i = 1; i < count; i++)
        {
            (this->*drawLineAlgorithm)(x[i - 1], y[i - 1], x[i], y[i], texture, polygon.borderColor[0], polygon.borderColor[1], polygon.borderColor[2], polygon.borderColor[3]);
            drawPoint(x[i], y[i], texture, polygon.borderColor[0], polygon.borderColor[1], polygon.borderColor[2], polygon.borderColor[3]);
        }
        for(auto p1: polygon.getVerticalAndHorizontalPoints())
        {
            drawPoint(p1.getX(), p1.getY(), texture, 0, 0, 255, 255);
        }
    }
}

bool Window::handleVertexClick(int x, int y)
{
    for (int i = 0; i < state.figures.size(); i++)
    {
        state.draggedVertex = state.figures[i].getClickedVertex(x, y, Window::epsilon);
        if(state.draggedVertex != nullptr)
        {
            state.draggedPolygon = &(state.figures[i]);
            return true; 
        }
    }
    return false;
}

void Window::handleMouseButtonUp()
{
    state.leftButtonDown = false;
    state.draggedPolygon = nullptr;
    state.draggedVertex = nullptr;
    state.figuresChanged = false;
}

void Window::clearTexture(SDL_Texture* texture)
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void Window::drawBresenhamLine(int x1, int y1, int x2, int y2, SDL_Texture* texture, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_SetRenderTarget(renderer, texture);
    int d, dx, dy, ai, bi, xi, yi;
    int x = x1, y = y1;
    if (x1 < x2)
    {
        xi = 1;
        dx = x2 - x1;
    }
    else
    {
        xi = -1;
        dx = x1 - x2;
    }
    if (y1 < y2)
    {
        yi = 1;
        dy = y2 - y1;
    }
    else
    {
        yi = -1;
        dy = y1 - y2;
    }
    SDL_RenderDrawPoint(renderer, x, y);
    if (dx > dy)
    {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;
        while (x != x2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                x += xi;
            }
            SDL_RenderDrawPoint(renderer, x, y);

        }
    }
    else
    {
        ai = ( dx - dy ) * 2;
        bi = dx * 2;
        d = bi - dy;
        while (y != y2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                y += yi;
            }
            SDL_RenderDrawPoint(renderer, x, y);
            
        }
    }
 }