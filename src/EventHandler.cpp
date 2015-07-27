//
//  event.cpp
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#include <iostream>
#include "EventHandler.h"
#include "World/Cat.h"
#include "Camera.h"
#include "FatalGameException.h"

using namespace std;

EventHandler::EventHandler(Game *game) {
    this->game = game;
    bindings = new map<SDL_Keycode, int>();
    accumulatedMotion.x = 0;
    accumulatedMotion.y = 0;

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

void EventHandler::bind(SDL_Keycode key, int action) {
    (*bindings)[key] = action;
}

void EventHandler::unbind(SDL_Keycode key) {
    (*bindings)[key] = NULL;
}

void EventHandler::unbindAll() {
    bindings->clear();
}

void EventHandler::onKeyDown(SDL_Keycode key) {
    if (bindings->find(key) != bindings->end()) {
        if ((*bindings)[key] == MOVEMENT_CYCLE) {
            game->cycleActive();
        } else if ((*bindings)[key] == MOVEMENT_CAMERA_RESET) {
            game->getCamera()->setTrackingMode(TRACK_LOOSE);
        } else {
            game->getActiveCat()->setMovement((*bindings)[key]);
        }
    }
}

void EventHandler::onKeyUp(SDL_Keycode key) {
    if (bindings->find(key) != bindings->end()) {
        game->getActiveCat()->unsetMovement((*bindings)[key]);
    }
}

void EventHandler::onMouseMotion(SDL_MouseMotionEvent motion) {
    if (ignoreMovement) {
        ignoreMovement = false;
        return;
    }

    accumulatedMotion.x += motion.xrel;
    accumulatedMotion.y += motion.yrel;

    game->getCamera()->setTrackingMode(TRACK_DISABLE);
}

void EventHandler::onMousePress(SDL_MouseButtonEvent event) {

}

void EventHandler::handle() {
    SDL_Event e;

    accumulatedMotion.x = 0;
    accumulatedMotion.y = 0;

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

    if (accumulatedMotion.x != 0 || accumulatedMotion.y != 0) {
        Camera *camera = game->getCamera();
        camera->origin.x += accumulatedMotion.x;
        camera->origin.y += accumulatedMotion.y;

        SDL_WarpMouseInWindow(game->window, game->originX, game->originY);

        ignoreMovement = true;
    }
}