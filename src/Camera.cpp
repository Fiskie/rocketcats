//
// Created by Fiskie on 26/05/15.
//

#include "Camera.h"
#include "FatalGameException.h"

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
    SDL_Renderer *renderer = game->renderer;
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
    SDL_Renderer *renderer = game->renderer;
    Map *map = game->map;

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
    int cameraX = frameOrigin.x - game->originX;
    int cameraY = frameOrigin.y - game->originY;
    int cameraW = frameOrigin.x + game->originX;
    int cameraH = frameOrigin.y + game->originY;

    renderLabel(format("Camera: origin (%d, %d) at (%d, %d, %d, %d)", frameOrigin.x, frameOrigin.y, cameraX, cameraY, cameraW, cameraH), 4, 124);

    Map *map = game->map;

    SDL_Renderer *renderer = game->renderer;

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
}

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b
#define clamp(a, b, c) min(a, max(b, c))

void Camera::renderMapTexture() {
    int cameraX = frameOrigin.x - game->originX;
    int cameraY = frameOrigin.y - game->originY;
    int cameraW = frameOrigin.x + game->originX;
    int cameraH = frameOrigin.y + game->originY;


    Map *map = game->map;

    SDL_Texture *texture = map->foreground;
    SDL_Renderer *renderer = game->renderer;

    SDL_Rect source;
    SDL_Rect destination;

    source.x = cameraX;
    source.y = cameraY;
    source.w = game->resX;
    source.h = game->resY;

    destination.x = max(0, -cameraX);
    destination.y = max(0, -cameraY);

    if (cameraX <= 0) {
        destination.w = min(game->resX, game->resX + cameraX);
    } else {
        destination.w = cameraX > game->resX ? game->resX - (cameraX - game->resX) : game->resX;
    }

    if (cameraY <= 0) {
        destination.h = min(game->resY, game->resY + cameraY);
    } else {
        destination.h = cameraY > game->resY ? game->resY - (cameraY - game->resY) : game->resY;
    }

    SDL_RenderCopy(renderer, texture, &source, &destination);

    renderLabel(format("Camera: origin (%d, %d) at (%d, %d, %d, %d)", frameOrigin.x, frameOrigin.y, cameraX, cameraY, cameraW, cameraH), 4, 124);
    renderLabel(format("Camera: source (%d, %d, %d, %d)", source.x, source.y, source.w, source.h), 4, 144);
    renderLabel(format("Camera: dest   (%d, %d, %d, %d)", destination.x, destination.y, destination.w, destination.h), 4, 164);
}

void Camera::render() {
    // For thread safety, store camera origin variables before we render them.
    AbsPos origin = getOrigin();
    frameOrigin = {origin.x, origin.y};

    long start = SDL_GetTicks();

    this->renderBg();
    long bgRender = SDL_GetTicks();

    this->renderMapTexture();
    long mapRender = SDL_GetTicks();

    list<Entity*> *entities = game->map->getEntities();

    for (auto it = entities->begin(); it != entities->end(); ++it) {
        (*it)->render(this);
    }

    long entityRender = SDL_GetTicks();

    renderLabel(format("bg render time: %d", bgRender - start), 4, 44);
    renderLabel(format("map render time: %d", mapRender - bgRender), 4, 64);
    renderLabel(format("entity render time: %d", entityRender - mapRender), 4, 84);
    renderLabel(format("average fps: %f", game->avgFPS), 4, 104);

    SDL_RenderPresent(game->renderer);
}

void Camera::placeOriginAtActiveCat() {
    Cat *cat = game->getActiveCat();

    origin.x = (int) cat->origin.x;
    origin.y = (int) cat->origin.y;
}