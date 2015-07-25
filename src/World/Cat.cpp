//
// Created by Fiskie on 16/07/2015.
//

#include "Cat.h"
#include "NinjaRope.h"
#include "../Game.h"
#include "Explosion.h"

Cat::Cat() {
    health = 100;
    height = 24;
    width = 12;
    velocity.x = 0;
    velocity.y = 0;
    unsetAllMovement();
}

Team *Cat::getTeam() {
    return team;
}

void Cat::setTeam(Team *team) {
    this->team = team;
}

char* Cat::getName() {
    return name;
}

void Cat::setName(char* name) {
    this->name = name;
}

void Cat::setName(const char* name) {
    this->name = (char*) name;
}

int Cat::getHealth() {
    return health;
}

Pos Cat::getOrigin() {
    return this->origin;
}

void Cat::setMovement(int movement) {
    movements[movement] = true;
}

void Cat::unsetMovement(int movement) {
    movements[movement] = false;
}

void Cat::unsetAllMovement() {
    for (int i = 0; i < 6; i++) {
        movements[i] = false;
    }
}

void Cat::move(int direction) {
    int base = origin.y + height / 2;
    int ceil = base + height;

    int desiredX = origin.x + (direction == DIRECTION_LEFT ? -1 : 1);
    int checkedX = desiredX + (direction == DIRECTION_LEFT ? -(width / 2) : width / 2);

    if (map->terrain[checkedX][base - 1].destroyed) {
        origin.x = desiredX;
    }

    applyGravity();
}

void Cat::applyVelocity() {
    // Move based on airborne velocity, then alter the velocity to account for gravity, friction, air resistance, bounce, etc.

    // Now, calculate for gravity, etc.
    if (velocity.y > maxFallSpeed) {
        // If we are above the natural fall speed, slowly lower it.
        // todo use an easing function
        velocity.y--;
    }

    if (isTouchingFloor()) {
        velocity.y = 0;

        // Allow velocity adjustments by walking.
        if (movements[MOVEMENT_LEFT]) {
            velocity.x = -maxWalkSpeed;
            orientation = DIRECTION_LEFT;
        } else if (movements[MOVEMENT_RIGHT]) {
            velocity.x = maxWalkSpeed;
            orientation = DIRECTION_RIGHT;
        } else {
            velocity.x *= 0.7;
        }

        if (movements[MOVEMENT_JUMP]) {
            velocity.x = (orientation == DIRECTION_LEFT ? -6 : 6);
            velocity.y = -12;
        }

        if (movements[MOVEMENT_JUMP2]) {
            velocity.x = (orientation == DIRECTION_LEFT ? 4 : -4);
            velocity.y = -20;
        }

        if (movements[MOVEMENT_ADJUSTAIMDOWN]) {
            aimAngle--;
        } else if (movements[MOVEMENT_ADJUSTAIMUP]) {
            aimAngle++;
        }

        if (movements[MOVEMENT_ACTION]) {
            Explosion* explosion = new Explosion(map, {(int) origin.x, (int) origin.y}, 50);
            explosion->explode();
            delete explosion;
        }
    } else {
        velocity.y++;
    }

    applyGravity();

    // Apply velocity to origin
    double remainingX = velocity.x;
    double remainingY = velocity.y;
    while (remainingX != 0 || remainingY != 0) {
        double addedX = (remainingX > 0 ? (remainingX > 1 ? 1 : remainingX) : (remainingX < -1 ? -1 : remainingX));
        double addedY = (remainingY > 0 ? (remainingY > 1 ? 1 : remainingY) : (remainingY < -1 ? -1 : remainingY));

        moveTo(origin.x + addedX, origin.y + addedY);

        remainingX -= addedX;
        remainingY -= addedY;
    }
}