//
// Created by Fiskie on 24/07/2015.
//

#ifndef ROCKETCATS_ENTITY_H
#define ROCKETCATS_ENTITY_H

#include "../Pos.h"
#include "Tile.h"
#include "Map.h"

class Map;

class Entity {
protected:
    int width, height;
    Map *map;
    double maxFallSpeed = 20;
public:
    Pos origin;
    Pos velocity;
    double fallSpeedTolerance = 15;
    double elevationTolerance = 5;

    void applyMovement();
    void applyGravity();
    virtual void applyVelocity() = 0;

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
