//
// Created by Fiskie on 17/07/2015.
//

#ifndef ROCKETCATS_TEAM_H
#define ROCKETCATS_TEAM_H

#include "Cat.h"
#include <vector>

using namespace std;

class Cat;

class Team {
private:
    vector<char*>* catNames;
    char* name;
public:
    vector<char *> *getCatNames() const {
        return catNames;
    }

    void setCatNames(vector<char *> *catNames) {
        Team::catNames = catNames;
    }

    char *getName() const {
        return name;
    }

    void setName(char *name) {
        Team::name = name;
    }

    void setName(const char *name) {
        Team::name = (char*) name;
    }

    void setCatName(int i, char* name);
    char* getCatName(int i);
    vector<Cat*>* generateCats();

    Team(Game *game);

    Game *game;
};


#endif //ROCKETCATS_TEAM_H
