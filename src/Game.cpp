//
//  game.cpp
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#include <SDL2_ttf/SDL_ttf.h>
#include "Game.h"
#include "FatalGameException.h"
#include "Cameras/Camera.h"
#include "World/MapGenerator.h"
#include "World/Cat.h"

Game::Game() {
    running = true;
}

void Game::initialize() {
    if (TTF_Init() < 0) {
        char err[] = "SDL_TTF could not initialize! Error: ";
        strcat(err, TTF_GetError());
        throw new FatalGameException(err);
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        char err[] = "SDL could not initialize! Error: ";
        strcat(err, SDL_GetError());
        throw new FatalGameException(err);
    }

    // Create window
    window = SDL_CreateWindow("rocketcats", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resX, resY,
                              SDL_WINDOW_SHOWN);

    if (window == NULL) {
        char err[] = "Window could not be created! Error: ";
        strcat(err, SDL_GetError());
        throw new FatalGameException(err);
    }

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        char err[] = "Renderer could not be created! Error: ";
        strcat(err, SDL_GetError());
        throw new FatalGameException(err);
    }

    setup();
}

void Game::run() {
    fpsTimer = new Timer();
    Timer *capTimer = new Timer();

    try {
        initialize();

        double delta = 0;
        long prev = SDL_GetTicks();

        const double UPDATE_RATE = 60;
        const double TICK_TIME = 1000 / UPDATE_RATE;

        frames = 0;

        fpsTimer->start();

        while (running) {
            capTimer->start();

            avgFPS = frames / ( fpsTimer->getTicks() / 1000.f );

            if (avgFPS > 2000000)
                avgFPS = 0;

            long now = SDL_GetTicks();
            long updateLength = now - prev;

            delta += ((double) updateLength / 1000);

            prev = now;

            event->handle();

            if (!running)
                break;

            update(delta);
            render();
            delta = 0;
            frames++;

            // If frame finished early
            int frameTicks = capTimer->getTicks();

            if (frameTicks < TICK_TIME)
            {
                //Wait remaining time
                SDL_Delay((Uint32) TICK_TIME - frameTicks);
            }
        }
    } catch (FatalGameException *ex) {
        printf("%s\n", ex->what());
    }

    onExit();
}

void Game::stop() {
    running = false;
}

void Game::onExit() {
    // Destroy window
    SDL_DestroyWindow(window);

    // Deallocate
    window = NULL;
    renderer = NULL;
    fpsTimer = NULL;

    delete event;
    delete camera;

    // Quit SDL subsystems
    SDL_Quit();
    TTF_Quit();
}

SDL_Window *Game::getWindow() {
    return window;
}

SDL_Renderer *Game::getRenderer() {
    return renderer;
}

void Game::setMap(Map *map) {
    this->map = map;
}

Map *Game::getMap() {
    return map;
}

void Game::render() {
    camera->render();
}

void Game::update(double delta) {
    auto entities = map->getEntities();

    for (auto i = entities->begin(); i != entities->end(); i++) {
        (*i)->onUpdate();
    }
}

void Game::setup() {
    fpsTimer->start();
    event = new Event(this);
    camera = new Camera(this);

    MapGenerator *generator = new MapGenerator();
    map = generator->generate();

    Cat *cat = new Cat();
    cat->setName("Meow");

    activeCat = cat;

    ((Camera *) camera)->target = cat;

    Cat *cat2 = new Cat();
    cat2->setName("Rrerr");

    Cat *cat3 = new Cat();
    cat3->setName("Nyan");

    Cat *cat4 = new Cat();
    cat4->setName("Mew");

    map->addCat(cat);
    map->addCat(cat2);
    map->addCat(cat3);
    map->addCat(cat4);

    map->placeCats();

    SDL_ShowCursor(0);
}

Game::~Game() {
    this->onExit();
}

void Game::setResolution(int x, int y) {
    this->resX = x;
    this->resY = y;
    this->originX = x / 2;
    this->originY = y / 2;
}

void Game::cycleActive() {
    auto cats = map->getCats();
    bool next = false;

    for (auto iter = cats->begin(); iter != cats->end(); iter++) {
        if (next) {
            activeCat = *iter;
            return;
        }

        if (*iter == activeCat)
            next = true;
    }

    activeCat = *cats->begin();
}
