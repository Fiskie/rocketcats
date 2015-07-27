//
// Created by Fiskie on 16/07/2015.
//

#ifndef ROCKETCATS_MAPGENERATOR_H
#define ROCKETCATS_MAPGENERATOR_H


#include "Map.h"

class MapGenerator {
    Game *game;
public:
    Map *generate();
    MapGenerator(Game *game);
};


#endif //ROCKETCATS_MAPGENERATOR_H
