//
// Created by Fiskie on 17/07/2015.
//

#include "Team.h"

Team::Team() {
    this->members = new list<Cat>();
}

list<Cat> *Team::getMembers() {
    return members;
}