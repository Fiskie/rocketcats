//
// Created by Fiskie on 16/07/2015.
//

#include "Map.h"
#include "Cat.h"

Map::Map(int w, int h) {
    this->width = w;
    this->height = h;
    this->cats = new list<Cat*>();
    this->entities = new list<Entity*>();
}

list<Cat*>* Map::getCats() {
    return cats;
}

void Map::addCat(Cat *cat) {
    entities->push_back(cat);
    cats->push_back(cat);
    cat->setMap(this);
}

void Map::placeCat(Cat *cat) {
    // Check random spot on the map
    bool placed = false;

    while (!placed) {
        double randX = rand() % width;
        double randY = rand() % height;

        Tile tile = getTile((int) randX, (int) randY);

        if (tile.destroyed) {
            // Snap to ground
            while (cat->isValidLocation(randX, randY)) {
                randY++;
            }

            cat->setOrigin({randX, randY});
            placed = true;
        }
    }
}

void Map::placeCats() {
    for (auto iter = cats->begin(); iter != cats->end(); iter++) {
        placeCat(*iter);
    }
}

/**
 * A safe way to fetch a tile that isn't going to be out of map bounds without causing a SIGSEGV
 * Returns null if the tile does not exist
 */
Tile Map::getTile(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return terrain[x][y];

    Tile tile;
    tile.outOfBounds = true;

    return tile;
}

void Map::setTile(int x, int y, Tile tile) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        terrain[x][y] = tile;
}