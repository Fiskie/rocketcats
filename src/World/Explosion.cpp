//
// Created by Fiskie on 21/07/2015.
//

#include "Explosion.h"
#include "../Pos.h"

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

    // project movable entities away from the explosion
    list<Entity*> *entities = map->getEntities();

    for (auto i = entities->begin(); i != entities->end(); i++) {
        // todo
        double relX = ((*i)->origin.x - pos.x);
        double relY = ((*i)->origin.y - pos.y);
        double distance = sqrt(relX * relX + relY * relY);

        if (abs((int) distance) <= radius) {
            // entity caught in explosion

            double velX = -((radius + relX) * force);
            double velY = -((radius + relY) * force);

            printf("Distance: %.2f Radius: %d Force: %.2f\n", distance, radius, force);
            printf("Rel: %.2f %.2f\n", relX, relY);
            printf("Velocity: %.2f %.2f\n", velX, velY);

            (*i)->setActive();
            (*i)->velocity.x = (abs(relX) < 1 ? 0 : velX);
            (*i)->velocity.y = (abs(relY) < 1 ? 0 : velY);
        }
    }
}

Explosion::Explosion(Map *map, AbsPos pos, int radius, double force) {
    this->map = map;
    this->pos = pos;
    this->radius = radius;
    this->force = force;
}

Explosion::Explosion(Map *map, AbsPos pos, int radius) {
    this->map = map;
    this->pos = pos;
    this->radius = radius;
    this->force = (double) radius / 75;
}