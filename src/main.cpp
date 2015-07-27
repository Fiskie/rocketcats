//
//  main.cpp
//  test
//
//  Created by Fiskie on 21/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#include "Game.h"

int main(int argc, const char *argv[]) {
    Game *game = new Game();

    game->setResolution(1600, 900);
    game->run();

    return 0;
}