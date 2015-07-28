//
// Created by Fiskie on 16/07/2015.
//

#ifndef ROCKETCATS_MAP_H
#define ROCKETCATS_MAP_H

#include <SDL2/SDL_pixels.h>
#include "Tile.h"
#include "Entity.h"
#include <list>

#define MAP_WIDTH 1600
#define MAP_HEIGHT 900

using namespace std;

class Cat;
class Entity;
class Game;

class Map {
private:
    list<Entity*> *entities;
    list<Cat*> *cats;
public:
    Game *game;
    int width;
    int height;
    Tile terrain[MAP_WIDTH][MAP_HEIGHT];
    list<Cat*>* getCats();
    void addCat(Cat* cat);

    void placeCats();

    void placeCat(Cat *cat);

    Tile getTile(int x, int y);

    void setTile(int x, int y, Tile tile);

    list<Entity*>* getEntities() {
        return entities;
    }

    void addEntity(Entity *entity);

    void updateTexture();

    SDL_Texture *foreground;
    Map(Game *game, int w, int h);

    void createTexture();

    void requestTextureUpdate();

    bool update;
};

#endif //ROCKETCATS_MAP_H
