//
// Created by Fiskie on 24/07/2015.
//

#ifndef ROCKETCATS_ENTITY_H
#define ROCKETCATS_ENTITY_H

#include <SDL2/SDL_render.h>
#include "../Pos.h"
#include "Tile.h"
#include "Map.h"

class Map;
class Camera;

class Entity {
protected:
    int width, height;
    Map *map;
    double maxFallSpeed = 20;
    // the active flag should be true if an entity should be affected by time, movement, gravity, etc.
    // this is so we don't have to check for movement on entities that should not be moving.
    bool active = true;
public:

    bool isActive() {
        return active;
    }

    void setActive() {
        active = true;
    }

    Pos origin;
    Pos velocity;
    double fallSpeedTolerance = 15;
    double elevationTolerance = 5;

    void applyMovement();
    void applyGravity();
    void applyVelocity();

    virtual void onUpdate() = 0;
    virtual void render(Camera *camera) = 0;

    bool isTouchingFloor();
    bool isTouchingCeiling();
    bool isValidLocation(double x, double y);
    bool moveTo(double x, double y);

    Map *getMap() const {
        return map;
    }

    void setMap(Map *map) {
        this->map = map;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int height) {
        this->height = height;
    }

    int getWidth() const {
        return width;
    }

    void setWidth(int width) {
        this->width = width;
    }
};

#endif //ROCKETCATS_ENTITY_H
