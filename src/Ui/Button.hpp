#ifndef __BUTTONHPP__
#define __BUTTONHPP__

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>


class Button
{
private:
    SDL_Rect rect;       
    SDL_Color color;     
    SDL_Texture* texture;   
    std::string buttonText;
    TTF_Font* font;

    const static int BUTTON_HEIGHT = 20;
    const static int BUTTON_WIDTH = 60;

public:
    Button(SDL_Renderer* renderer, int x, int y, std::string text, TTF_Font* font);
    bool isButtonClicked(int mouseX, int mouseY);
    void render(SDL_Renderer* renderer);
};

#endif