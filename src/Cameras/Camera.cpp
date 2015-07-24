//
// Created by Fiskie on 26/05/15.
//

#include "Camera.h"
#include "../FatalGameException.h"

Camera::Camera(Game *game) {
    this->game = game;
}

Camera::Camera() {
    font = TTF_OpenFont("OpenSans-Regular.ttf", 11);

    if (font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        throw new FatalGameException("Could not load required font.");
    }
}

void Camera::renderLabel(string label, int x, int y, int alignment) {
    SDL_Renderer *renderer = game->getRenderer();
    SDL_Color color;
    const char* str = label.c_str();
    int w;
    int h;

    color.b = 127;
    color.g = 255;
    color.r = 255;
    color.a = 0;

    SDL_Surface *text = TTF_RenderText_Blended(font, str, color);

    // Size our label decently
    TTF_SizeText(font, str, &w, &h);

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, text);

    SDL_FreeSurface(text);

    SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = w;
    SrcR.h = h;

    switch (alignment) {
        default:
            DestR.x = x;
            break;
        case ALIGNMENT_CENTER:
            DestR.x = x - w / 2;
            break;
        case ALIGNMENT_RIGHT:
            DestR.x = x - w;
            break;
    }

    DestR.y = y;
    DestR.w = w;
    DestR.h = h;

    SDL_RenderCopy(renderer, tex, &SrcR, &DestR);

    SDL_DestroyTexture(tex);
}

Camera::~Camera() {

}

void Camera::renderLabel(string label, int x, int y) {
    renderLabel(label, x, y, ALIGNMENT_LEFT);
}
