//
// Created by Fiskie on 25/07/2015.
//

#include "Mine.h"
#include "Explosion.h"
#include "Cat.h"

void Mine::detonate() {
    Explosion *explosion = new Explosion(map, {(int) origin.x, (int) origin.y}, 40);
    explosion->explode();
    delete explosion;
}

void Mine::onUpdate() {
    if (isTriggered()) {
        detonate();
    }
}

bool Mine::isTriggered() {
    list<Cat*> *cats = map->getCats();

    for (auto i = cats->begin(); i != cats->end(); i++) {
        double relX = ((*i)->origin.x - origin.x);
        double relY = ((*i)->origin.y - origin.y);
        double distance = sqrt(relX * relX + relY * relY);

        if (abs((int) distance) <= 40) {
            return true;
        }
    }

    return false;
}

void Mine::render(SDL_Renderer *renderer) {

}