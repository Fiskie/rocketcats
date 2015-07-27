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

    double currentTime = SDL_GetTicks();

    if (placedTimestamp + this->placeTime > currentTime) {
        return;
    }

    if ((triggeredTimestamp != -1 || isTriggered()) && !detonated) {
        if (triggeredTimestamp == -1) {
            triggeredTimestamp = currentTime;
        }

        if (currentTime > triggeredTimestamp + this->fuseTime) {
            detonate();
        }
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

    AbsPos camOrigin = camera->getFrameOrigin();

    int cameraX = camOrigin.x - game->originX;
    int cameraY = camOrigin.y - game->originY;

    SDL_Renderer *renderer = game->renderer;

    int oX = (int) origin.x, oY = (int) origin.y;

    SDL_Rect rect;
    rect.x = oX - width / 2 - cameraX;
    rect.y = oY - height / 2 - cameraY;
    rect.w = width;
    rect.h = height;

    SDL_Color bg;

    if (triggeredTimestamp == -1) {
        bg.b = 25;
        bg.g = 120;
        bg.r = 25;
    } else {
        bg.b = 25;
        bg.g = 25;
        bg.r = 120;
    }

    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * Constructor for mine placed by map generator
 */
Mine::Mine(Game *game) {
    this->game = game;
    init();
}

/**
 * Constructor for mine placed by
 */
Mine::Mine(Game *game, Cat *owner) {
    this->game = game;
    this->owner = owner;
    placedTimestamp = SDL_GetTicks();
    init();
}

void Mine::init() {
    width = 12;
    height = 6;
    this->placedTimestamp = SDL_GetTicks();
}