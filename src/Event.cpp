//
//  event.cpp
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#include <iostream>
#include "Event.h"
#include "World/Cat.h"
#include "Camera.h"

using namespace std;

Event::Event(Game *game) {
    this->game = game;
    bindings = new map<SDL_Keycode, int>();

    bind(SDLK_w, MOVEMENT_ADJUSTAIMUP);
    bind(SDLK_s, MOVEMENT_ADJUSTAIMDOWN);
    bind(SDLK_a, MOVEMENT_LEFT);
    bind(SDLK_d, MOVEMENT_RIGHT);
    bind(SDLK_RETURN, MOVEMENT_JUMP);
    bind(SDLK_BACKSPACE, MOVEMENT_JUMP2);
    bind(SDLK_SPACE, MOVEMENT_ACTION);
    bind(SDLK_TAB, MOVEMENT_CYCLE);
    bind(SDLK_h, MOVEMENT_CAMERA_RESET);
}

void Event::bind(SDL_Keycode key, int action) {
    (*bindings)[key] = action;
}

void Event::unbind(SDL_Keycode key) {
    (*bindings)[key] = NULL;
}

void Event::unbindAll() {
    bindings->clear();
}

void Event::onKeyDown(SDL_Keycode key) {
    if (bindings->find(key) != bindings->end()) {
        if ((*bindings)[key] == MOVEMENT_CYCLE) {
            game->cycleActive();
        } else if ((*bindings)[key] == MOVEMENT_CAMERA_RESET) {
            ((Camera*) game->getCamera())->setTrackingMode(TRACK_LOOSE);
        } else {
            game->getActiveCat()->setMovement((*bindings)[key]);
        }
    }
}

void Event::onKeyUp(SDL_Keycode key) {
    if (bindings->find(key) != bindings->end()) {
        game->getActiveCat()->unsetMovement((*bindings)[key]);
    }
}

void Event::onMouseMotion(SDL_MouseMotionEvent motion) {
    // Warping the mouse back to the middle of the window causes another event to fire; this stops that.
    if (ignoreMovement) {
        ignoreMovement = false;
        return;
    }

    Camera *camera = game->getCamera();

    camera->setTrackingMode(TRACK_DISABLE);

    camera->origin.x += motion.xrel;
    camera->origin.y += motion.yrel;

    SDL_WarpMouseInWindow(game->getWindow(), game->originX, game->originY);

    ignoreMovement = true;
}

void Event::onMousePress(SDL_MouseButtonEvent event) {

}

void Event::handle() {
    SDL_Event e;

    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                game->stop();
                break;
            case SDL_KEYDOWN:
                onKeyDown(e.key.keysym.sym);
                break;
            case SDL_KEYUP:
                onKeyUp(e.key.keysym.sym);
                break;
            case SDL_MOUSEMOTION:
                onMouseMotion(e.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                onMousePress(e.button);
                break;
            default:
                break;
        }
    }
}