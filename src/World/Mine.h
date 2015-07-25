//
// Created by Fiskie on 25/07/2015.
//

#ifndef ROCKETCATS_MINE_H
#define ROCKETCATS_MINE_H


#include "Entity.h"

class Mine: public Entity {
    bool isTriggered();
    void detonate();
    void onUpdate();

    void render(SDL_Renderer *renderer);
};


#endif //ROCKETCATS_MINE_H
