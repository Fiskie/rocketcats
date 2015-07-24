//
// Created by Fiskie on 17/07/2015.
//

#ifndef ROCKETCATS_TEAM_H
#define ROCKETCATS_TEAM_H

#include "Cat.h"
#include <list>

using namespace std;

class Cat;

class Team {
private:
    list<Cat> *members;
public:
    list<Cat> *getMembers();
    Team();
};


#endif //ROCKETCATS_TEAM_H
