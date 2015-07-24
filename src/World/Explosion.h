//
// Created by Fiskie on 21/07/2015.
//

#ifndef ROCKETCATS_EXPLOSION_H
#define ROCKETCATS_EXPLOSION_H


#include "../Game.h"
#include "../Pos.h"

class Explosion {
private:
    AbsPos pos;
    Map *map;
public:
    Explosion(Map *map, AbsPos pos, int radius);
    Explosion(Map *map, AbsPos pos, int radius, double force);
    void explode();
    int radius;
    double force;
};


#endif //ROCKETCATS_EXPLOSION_H
