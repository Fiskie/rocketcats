//
// Created by Fiskie on 25/07/2015.
//

#ifndef ROCKETCATS_MINE_H
#define ROCKETCATS_MINE_H

#include "Entity.h"

class Mine: public Entity {
private:
    bool detonated = false;

public:
    Mine(Game* game);
    bool isTriggered();
    void detonate();
    void onUpdate();
    void render(Camera *camera);
};


#endif //ROCKETCATS_MINE_H
