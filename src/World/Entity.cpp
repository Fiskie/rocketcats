//
// Created by Fiskie on 24/07/2015.
//

#include "Entity.h"

void Entity::applyVelocity() {
    // Move based on airborne velocity, then alter the velocity to account for gravity, friction, air resistance, bounce, etc.

    /*
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
            velocity.x = 0;
        }

        if (movements[MOVEMENT_JUMP]) {
            velocity.x = (orientation == DIRECTION_LEFT ? -8 : 8);
            velocity.y = -15;
        }

        if (movements[MOVEMENT_JUMP2]) {
            velocity.x = (orientation == DIRECTION_LEFT ? 4 : -4);
            velocity.y = -25;
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
    }*/

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

    if (!removable && origin.y > map->height + 256) {
        printf("Object fell out of world, marking as removeable.\n");
        removable = true;
    }
}

bool Entity::moveTo(double x, double y) {
    if (isValidLocation(x, y)) {
        origin.x = x;
        origin.y = y;
        return true;
    }

    // If it's not a valid location, we might be trying to walk up a hill
    // Move up the hill in accordance with the elevation tolerance
    for (int i = 1; i <= elevationTolerance; i++) {
        if (isValidLocation(x, y - i)) {
            origin.x = x;
            origin.y = y - i;
            return true;
        }
    }

    return false;
}

bool Entity::isValidLocation(double x, double y) {
    // Create our bounding box
    int ceil = (int) (y - height / 2);
    int base = ceil + height;
    int left = (int) (x - width / 2);
    int right = left + width;

    for (int checkedX = left; checkedX <= right; checkedX++) {
        Tile baseTile = map->getTile(checkedX, base);
        Tile ceilTile = map->getTile(checkedX, ceil);

        if (!baseTile.destroyed && !ceilTile.destroyed)
            return false;
    }

    for (int checkedY = ceil + 1; checkedY < base; checkedY++) {
        Tile leftTile = map->getTile(left, checkedY);
        Tile rightTile = map->getTile(right, checkedY);

        if (!leftTile.destroyed && !rightTile.destroyed)
            return false;
    }

    return true;
}

bool Entity::isTouchingFloor() {
    int base = (int) origin.y + height / 2 + 1;
    int startX = (int) origin.x - width / 2;
    int endX = startX + width;

    for (int x = startX; x <= endX; x++) {
        Tile tile = map->getTile(x, base);

        if (!tile.destroyed && !tile.outOfBounds) {
            return true;
        }
    }

    return false;
}

bool Entity::isTouchingCeiling() {
    int ceil = (int) origin.y - height / 2 - 1;
    int startX = (int) origin.x - width / 2;
    int endX = startX + width;

    for (int x = startX; x <= endX; x++) {
        Tile tile = map->getTile(x, ceil);

        if (!tile.destroyed && !tile.outOfBounds) {
            return true;
        }
    }

    return false;
}

void Entity::applyGravity() {
    // todo: alter velocity
    // all this function does right now is basically snap you to the ground

    if (!isTouchingFloor()) {
        if (velocity.y <= maxFallSpeed)
            velocity.y++;
        else
            velocity.y = maxFallSpeed;

        velocity.x *= 0.99;
    }
}

void Entity::applyMovement() {
    applyGravity();
}

Entity::Entity() {
    velocity.x = 0;
    velocity.y = 0;
    origin.x = 0;
    origin.y = 0;
}
