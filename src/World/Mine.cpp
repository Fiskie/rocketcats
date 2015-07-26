//
// Created by Fiskie on 25/07/2015.
//

#include "Mine.h"
#include "Explosion.h"
#include "Cat.h"
#include "../Camera.h"

void Mine::detonate() {
    Explosion *explosion = new Explosion(map, {(int) origin.x, (int) origin.y}, 40);
    explosion->explode();
    delete explosion;
    detonated = true;
}

void Mine::onUpdate() {
    applyVelocity();

    if (isTriggered() && !detonated) {
        detonate();
    }
}

bool Mine::isTriggered() {
    list<Cat*> *cats = map->getCats();

    for (auto i = cats->begin(); i != cats->end(); i++) {
        double relX = ((*i)->origin.x - origin.x);
        double relY = ((*i)->origin.y - origin.y);
        double distance = sqrt(relX * relX + relY * relY);

        if (abs((int) distance) <= 20)
            return true;
    }

    return false;
}

void Mine::render(Camera *camera) {
    if (detonated) // todo entity cleanup
        return;

    AbsPos camOrigin = camera->origin;

    int cameraX = camOrigin.x - camera->game->originX;
    int cameraY = camOrigin.y - camera->game->originY;

    SDL_Renderer *renderer = camera->game->renderer;

    int oX = (int) origin.x, oY = (int) origin.y;

    SDL_Rect rect;
    rect.x = oX - width / 2 - cameraX;
    rect.y = oY - height / 2 - cameraY;
    rect.w = width;
    rect.h = height;

    SDL_Color bg;
    bg.b = 25;
    bg.g = 120;
    bg.r = 25;

    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &rect);
}

Mine::Mine(Game *game) {
    this->game = game;
    width = 12;
    height = 6;
}
