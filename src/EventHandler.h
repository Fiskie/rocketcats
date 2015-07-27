//
//  event.h
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#ifndef __rocketcats__events__
#define __rocketcats__events__

#include <stdio.h>
#include <map>
#include "Game.h"
#include "Pos.h"

using namespace std;

class Game;

class EventHandler {
private:
    Game *game;
    map<SDL_Keycode, int> *bindings;
    AbsPos accumulatedMotion;
public:
    EventHandler(Game *game);

    void handle();

    void onKeyDown(SDL_Keycode key);

    void onMouseMotion(SDL_MouseMotionEvent motion);

    void onMousePress(SDL_MouseButtonEvent event);

    void onKeyUp(SDL_Keycode key);

    bool ignoreMovement = false;

    void bind(SDL_Keycode key, int action);

    void unbind(SDL_Keycode key);

    void unbindAll();
};

#endif //__rocketcats__events__
