//
// Created by Fiskie on 17/07/2015.
//

#include <vector>
#include "Team.h"

Team::Team(Game *game) {
    this->game = game;
    this->catNames = new vector<char*>(8);

    for (int i = 0; i < 8; i++) {
        catNames->at(i) = "Unnamed";
    }
}

vector<Cat *>* Team::generateCats() {
    vector<Cat *> *cats = new vector<Cat *>();

    for (auto i = catNames->begin(); i != catNames->end(); i++) {
        Cat* cat = new Cat(game);
        cat->setTeam(this);
        cat->setName(*i);

        cats->push_back(cat);
    }

    return cats;
}

void Team::setCatName(int i, char *name) {
    catNames->at(i) = name;
}

char* Team::getCatName(int i) {
    return catNames->at(i);
}
