//
// Created by Fiskie on 18/07/2015.
//

#ifndef ROCKETCATS_NINJAROPE_H
#define ROCKETCATS_NINJAROPE_H


#include "Cat.h"

class NinjaRope {
private:
    Cat owner;
    Pos anchor;
public:
    const Cat &getOwner() const {
        return owner;
    }

    void setOwner(const Cat &owner) {
        this->owner = owner;
    }

    const Pos &getAnchor() const {
        return anchor;
    }

    void setAnchor(const Pos &anchor) {
        this->anchor = anchor;
    }
};


#endif //ROCKETCATS_NINJAROPE_H
