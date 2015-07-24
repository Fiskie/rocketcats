//
// Created by Fiskie on 21/07/2015.
//

#ifndef ROCKETCATS_TILE_H
#define ROCKETCATS_TILE_H

#include <SDL2/SDL_pixels.h>

struct Tile {
public:
    SDL_Color color;
    bool indestructible = false;
    bool destroyed = true;
    bool outOfBounds = false;
};

#endif //ROCKETCATS_TILE_H
