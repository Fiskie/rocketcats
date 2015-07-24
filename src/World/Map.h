//
// Created by Fiskie on 16/07/2015.
//

#ifndef ROCKETCATS_MAP_H
#define ROCKETCATS_MAP_H

#include <SDL2/SDL_pixels.h>
#include "Tile.h"
#include <list>

using namespace std;

class Cat;

class Map {
private:
    list<Cat*> *cats;
public:
    int width;
    int height;
    Tile terrain[1600][900];
    Map(int w, int h);
    list<Cat*>* getCats();
    void addCat(Cat* cat);

    void placeCats();

    void placeCat(Cat *cat);

    Tile getTile(int x, int y);

    void setTile(int x, int y, Tile tile);
};

#endif //ROCKETCATS_MAP_H
