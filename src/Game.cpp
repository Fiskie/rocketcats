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
#include "Camera.h"
#include "World/MapGenerator.h"
#include "World/Cat.h"
#include "World/Mine.h"

Game::Game() {
    running = true;
}

void Game::initLibraries() {
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
}

void Game::initRenderer() {
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

int renderLoop(void *gamePtr) {
    Game* game = (Game*) gamePtr;

    game->initRenderer();

    printf("Running rendering thread..\n");

    Timer *capTimer = new Timer();
    Timer *fpsTimer = new Timer();
    fpsTimer->start();

    const double RENDER_RATE = 60;
    const double TICK_TIME = 1000 / RENDER_RATE;

    while (game->running) {
        capTimer->start();
        game->avgFPS = game->frames / (fpsTimer->getTicks() / 1000.f );

        if (game->frames % 100 == 0) {
            // Just to make the FPS calc a bit more accurate
            game->frames = 0;
            fpsTimer->start();
        }

        game->render();

        // If frame finished early
        int frameTicks = capTimer->getTicks();

        if (frameTicks < TICK_TIME)
        {
            //Wait remaining time
            SDL_Delay((Uint32) TICK_TIME - frameTicks);
        }

        game->frames++;
    }

    return 0;
}

void Game::main() {
    double t = 0.0;
    const double dt = 0.01;

    double currentTime = SDL_GetTicks();
    double accumulator = 0.0;
    frames = 0;
    Timer *capTimer = new Timer();

    SDL_Thread* renderThread = SDL_CreateThread(renderLoop, "RenderThread", (void *) this);

    const double UPDATE_RATE = 45;
    const double TICK_TIME = 1000 / UPDATE_RATE;

    while (running) {
        capTimer->start();

        double newTime = SDL_GetTicks();
        double frameTime = newTime - currentTime;

        if (frameTime > 0.01)
            frameTime = 0.01;

        currentTime = newTime;

        accumulator += frameTime;

        event->handle();

        //while (accumulator >= dt) {
            update();
        //    t += dt;
        //    accumulator -= dt;
        //}

        // If frame finished early
        int frameTicks = capTimer->getTicks();

        if (frameTicks < TICK_TIME)
        {
            //Wait remaining time
            SDL_Delay((Uint32) TICK_TIME - frameTicks);
        }

        // const double alpha = accumulator / dt;
    }

    SDL_WaitThread(renderThread, NULL);
}

void Game::run() {
    try {
        initLibraries();
        setup();
        main();
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

void Game::update() {
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

    Cat *cat = new Cat(this);
    cat->setName("Meow");

    activeCat = cat;

    camera->target = cat;

    Cat *cat2 = new Cat(this);
    cat2->setName("Rrerr");

    Cat *cat3 = new Cat(this);
    cat3->setName("Nyan");

    Cat *cat4 = new Cat(this);
    cat4->setName("Mew");

    map->addCat(cat);
    map->addCat(cat2);
    map->addCat(cat3);
    map->addCat(cat4);

    for (int i = 1; i < 10; i++) {
        Mine *mine = new Mine(this);
        mine->origin.x = i * 200;
        mine->origin.y = 50;

        map->addEntity(mine);
    }

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
