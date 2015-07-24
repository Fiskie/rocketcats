//
// Created by Fiskie on 16/07/2015.
//

#include <iostream>
#include "DefaultCamera.h"
#include "../Game.h"
#include "../World/Cat.h"
#include "../Pos.h"

void DefaultCamera::renderBg() {
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

AbsPos DefaultCamera::getOrigin() {
    Cat *target = game->getActiveCat();

    switch (trackingMode) {
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

void DefaultCamera::renderMap() {
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

void DefaultCamera::renderCat(Cat *cat) {
    AbsPos origin = getOrigin();

    int cameraX = origin.x - game->originX;
    int cameraY = origin.y - game->originY;

    SDL_Renderer *renderer = game->getRenderer();

    int catWidth = cat->getWidth(), catHeight = cat->getHeight();
    int oX = (int) cat->getOrigin().x, oY = (int) cat->getOrigin().y;

    SDL_Rect rect;
    rect.x = oX - catWidth / 2 - cameraX;
    rect.y = oY - catHeight / 2 - cameraY;
    rect.w = catWidth;
    rect.h = catHeight;

    SDL_Color bg;
    bg.b = 25;
    bg.g = 50;
    bg.r = 120;

    renderLabel(cat->getName(), oX - cameraX, oY - 40 - cameraY, ALIGNMENT_CENTER);
    renderLabel(format("%d", cat->getHealth()), oX - cameraX, oY - 28 - cameraY, ALIGNMENT_CENTER);

    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &rect);

    if (cat == game->getActiveCat()) {
        renderLabel(format("Position: %.2f %.2f", cat->origin.x, cat->origin.y), 4, 4);
        renderLabel(format("Velocity: %.2f %.2f", cat->velocity.x, cat->velocity.y), 4, 24);
    }
}

void DefaultCamera::render() {
    long start = SDL_GetTicks();

    this->renderBg();
    long bgRender = SDL_GetTicks();

    this->renderMap();
    long mapRender = SDL_GetTicks();

    list<Cat*> *cats = game->getMap()->getCats();

    for (auto it = cats->begin(); it != cats->end(); ++it) {
        this->renderCat(*it);
    }
    long catRender = SDL_GetTicks();

    renderLabel(format("bg render time: %d", bgRender - start), 4, 44);
    renderLabel(format("map render time: %d", mapRender - bgRender), 4, 64);
    renderLabel(format("cat render time: %d", catRender - mapRender), 4, 84);
    renderLabel(format("average fps: %f", game->avgFPS), 4, 104);

    SDL_RenderPresent(game->getRenderer());
}

DefaultCamera::DefaultCamera(Game *game) {
    this->game = game;
    origin.x = 0;
    origin.y = 0;
    zoom = 1;
}

void DefaultCamera::placeOriginAtActiveCat() {
    Cat *cat = game->getActiveCat();

    origin.x = (int) cat->origin.x;
    origin.y = (int) cat->origin.y;
}
