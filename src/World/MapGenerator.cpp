//
// Created by Fiskie on 16/07/2015.
//

#include <iostream>
#include <SDL2/SDL_timer.h>
#include "MapGenerator.h"

/**
 * Here I've copied some pseudo-code and converted them into C++ class methods, because I'm shit at maths.
 */

/**
 * Perform linear interpolation.
 */
double linearInterpolate(int a, int b, double progress) {
    return a * (1 - progress) + b * progress;
}

/**
 * Perform cosine interpolation. This allows for a smooth curve between points, but does not take into account the
 * position of neighbouring points.
 *
 * a: The point a.
 * b: The point b.
 * x: The point between a and b that were are calculating the position for.
 */
double cosineInterpolate(int a, int b, double progress) {
    double ft = progress * 3.1415927;
    double f = (1 - cos(ft)) * 0.5;

    return a * (1-f) + b * f;
}

/**
 * Perform cubic interpolation. This takes into account the position of points before the points that we're finding a
 * position for a point in between for, allowing for smoother curves that take neighbouring points into account.
 *
 * beforeA: The point before a.
 * a: The point a.
 * b: The point b.
 * beforeB: The point after b.
 * x: The point between a and b that were are calculating the position for.
 */
double cubicInterpolate(int beforeA, int a, int b, int afterB, int progress) {
    int P = (afterB - b) - (beforeA - a);
    int Q = (beforeA - a) - P;
    int R = b - beforeA;

    return P * pow(progress, 3) + Q * pow(progress, 2) + R * progress + a;
}

Map *MapGenerator::generate() {
    std::cout << "Generating... ";

    double startTime = SDL_GetTicks();

    int w = 800, h = 400;

    Map *map = new Map(game, w, h);

    Tile tile;

    tile.color.r = 40;
    tile.color.g = 60;
    tile.color.b = 40;
    tile.destroyed = false;

    for (int x = 0; x < w; x++) {
        int ceil = (int) cosineInterpolate(64, 284, (double) x / w);

        printf("x: %d ceil: %d\n", x, ceil);

        for (int y = h - 1; y > ceil; y--) {
            map->terrain[x][y] = tile;
        }
    }

    double endTime = SDL_GetTicks();

    std::cout << "Done. Time taken: " << (endTime - startTime) << "ms\n";

    return map;
}

MapGenerator::MapGenerator(Game *game) {
    this->game = game;
}
