//
// Created by Fiskie on 21/07/2015.
//

#include "Explosion.h"

Explosion::Explosion(Map *map, AbsPos pos, int radius) {
    this->map = map;
    this->pos = pos;
    this->radius = radius;
}

// Basically destroy everything in a circle around pos
void Explosion::explode() {
    // I don't know how this works, but it does, and that's what's important
    // Stolen from stackoverflow
    int r2 = radius * radius;
    int area = r2 << 2;
    int rr = radius << 1;

    for (int i = 0; i < area; i++) {
        int tx = (i % rr) - radius;
        int ty = (i / rr) - radius;

        if (tx * tx + ty * ty <= r2) {
            Tile tile = map->getTile(pos.x + tx, pos.y + ty);
            tile.destroyed = true;
            map->setTile(pos.x + tx, pos.y + ty, tile);
        }
    }
}
