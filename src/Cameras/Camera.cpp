//
// Created by Fiskie on 26/05/15.
//

#include "Camera.h"
#include "../FatalGameException.h"

Camera::Camera(Game *game) {
    this->game = game;
    origin.x = 0;
    origin.y = 0;
    zoom = 1;

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


void Camera::renderBg() {
    SDL_Renderer *renderer = game->getRenderer();
    Map *map = game->getMap();

    // Render background
    SDL_Color bg;
    bg.b = 25;
    bg.g = 0;
    bg.r = 0;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = map->width;
    rect.h = map->height;

    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &rect);
}

AbsPos Camera::getOrigin() {
    Cat *target = game->getActiveCat();

    switch (trackingMode) {
        default:
            break;
        case TRACK_STRICT:
            origin.x = (int) target->origin.x;
            origin.y = (int) target->origin.y;
            break;
        case TRACK_LOOSE:
            // Follow the target loosely
            int boundRadiusX = game->resX / 4;
            int boundRadiusY = game->resY / 4;

            if (target->origin.x > origin.x + boundRadiusX) {
                origin.x = (int) target->origin.x - boundRadiusX;
            } else if (target->origin.x < origin.x - boundRadiusX ) {
                origin.x = (int) target->origin.x + boundRadiusX;
            }

            if (target->origin.y > origin.y + boundRadiusY) {
                origin.y = (int) target->origin.y - boundRadiusY;
            } else if (target->origin.y < origin.y - boundRadiusY ) {
                origin.y = (int) target->origin.y + boundRadiusY;
            }
            break;
    }

    return origin;
}

void Camera::renderMap() {
    AbsPos origin = getOrigin();

    int cameraX = origin.x - game->originX;
    int cameraY = origin.y - game->originY;
    int cameraW = origin.x + game->originX;
    int cameraH = origin.y + game->originY;

    renderLabel(format("Camera: origin (%d, %d) at (%d, %d, %d, %d)", origin.x, origin.y, cameraX, cameraY, cameraW, cameraH), 4, 124);

    Map *map = game->getMap();

    SDL_Renderer *renderer = game->getRenderer();

    // Render tiles
    for (int x = cameraX; x < cameraW; x++) {
        for (int y = cameraY; y < cameraH; y++) {
            Tile tile = map->getTile(x, y);

            if (!tile.destroyed) {
                SDL_SetRenderDrawColor(renderer, tile.color.r, tile.color.g, tile.color.b, tile.color.a);
                SDL_RenderDrawPoint(renderer, x - cameraX, y - cameraY);
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
}

void Camera::render() {
    long start = SDL_GetTicks();

    this->renderBg();
    long bgRender = SDL_GetTicks();

    this->renderMap();
    long mapRender = SDL_GetTicks();

    list<Entity*> *entities = game->getMap()->getEntities();

    for (auto it = entities->begin(); it != entities->end(); ++it) {
        (*it)->render(this);
    }

    long entityRender = SDL_GetTicks();

    renderLabel(format("bg render time: %d", bgRender - start), 4, 44);
    renderLabel(format("map render time: %d", mapRender - bgRender), 4, 64);
    renderLabel(format("entity render time: %d", entityRender - mapRender), 4, 84);
    renderLabel(format("average fps: %f", game->avgFPS), 4, 104);

    SDL_RenderPresent(game->getRenderer());
}

void Camera::placeOriginAtActiveCat() {
    Cat *cat = game->getActiveCat();

    origin.x = (int) cat->origin.x;
    origin.y = (int) cat->origin.y;
}