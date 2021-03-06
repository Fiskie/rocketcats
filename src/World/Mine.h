//
// Created by Fiskie on 25/07/2015.
//

#ifndef ROCKETCATS_MINE_H
#define ROCKETCATS_MINE_H

#include "Entity.h"

class Mine: public Entity {
private:
    bool detonated = false;
    double placedTimestamp = -1;
    double triggeredTimestamp = -1;
    int fuseTime = 1000;
    int placeTime = 3000;
public:
    Mine(Game* game);
    Mine(Game* game, Cat*owner);
    bool isTriggered();
    void detonate();
    void onUpdate();
    void render(Camera *camera);
    void init();

    Cat *owner;

    void isPlaceCooldowned();
};


#endif //ROCKETCATS_MINE_H
