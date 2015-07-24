//
// Created by Fiskie on 16/07/2015.
//

#ifndef ROCKETCATS_CAT_H
#define ROCKETCATS_CAT_H

#define MOVEMENT_LEFT 1
#define MOVEMENT_RIGHT 2
#define MOVEMENT_JUMP 3
#define MOVEMENT_JUMP2 4
#define MOVEMENT_ACTION 5
#define MOVEMENT_ADJUSTAIMUP 6
#define MOVEMENT_ADJUSTAIMDOWN 7
#define MOVEMENT_CYCLE 8
#define MOVEMENT_CAMERA_RESET 9

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1

#include "Entity.h"
#include "Team.h"

class Team;

class Map;

class Cat: public Entity {
private:
    int health;
    char* name;
    Team *team;
    double maxWalkSpeed = 3;
    bool movements[8];
    double aimAngle = 0;
    int orientation = DIRECTION_RIGHT;
public:
    Cat();
    Team* getTeam();
    void setTeam(Team *team);
    char* getName();
    int getHealth();
    Pos getOrigin();

    void setName(char* name);
    void setName(const char *name);

    void move(int direction);

    void applyVelocity();

    void setMovement(int movement);
    void unsetMovement(int movement);
    void unsetAllMovement();

    void setOrigin(const Pos &origin) {
        this->origin = origin;
    }

    void setHealth(int health) {
        this->health = health;
    }
};


#endif //ROCKETCATS_CAT_H
