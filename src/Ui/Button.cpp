#include "Button.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Button::Button(SDL_Renderer* renderer, int x, int y, std::string text, TTF_Font* font)
{
    rect.x = x;
    rect.y = y;
    rect.w = Button::BUTTON_WIDTH;
    rect.h = Button::BUTTON_HEIGHT;

    color.a = 255;
    color.r = 255;
    color.g = 255;
    color.b = 255;

    SDL_Color textColor{0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    buttonText = text;
}

bool Button::isButtonClicked(int mouseX, int mouseY) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

void Button::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}