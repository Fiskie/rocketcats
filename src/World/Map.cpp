//
// Created by Fiskie on 16/07/2015.
//

#include "Map.h"
#include "Cat.h"

Map::Map(Game *game, int w, int h) {
    this->game = game;
    this->width = w;
    this->height = h;
    this->cats = new list<Cat*>();
    this->entities = new list<Entity*>();
}

list<Cat*>* Map::getCats() {
    return cats;
}

void Map::createTexture() {
    this->foreground = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

// Todo: this
void Map::updateTexture() {
    update = false;

    if (this->foreground == NULL)
        createTexture();

    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    printf("%s", SDL_GetError());

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Tile tile = terrain[x][y];

            if (!tile.destroyed)
                ((Uint32 *) surface->pixels)[y * width + x] = SDL_MapRGBA(surface->format, tile.color.a, tile.color.r, tile.color.g, tile.color.b);
        }
    }

    SDL_UpdateTexture(foreground, NULL, surface->pixels, surface->pitch);
    SDL_FreeSurface(surface);
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

void Map::addEntity(Entity *entity) {
    entities->push_back(entity);
    entity->setMap(this);
}

void Map::requestTextureUpdate() {
    update = true;
}
