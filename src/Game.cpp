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

    // Create window
    window = SDL_CreateWindow("Rocket Cats!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resX, resY,
                              SDL_WINDOW_SHOWN);

    if (window == NULL) {
        char err[] = "Window could not be created! Error: ";
        strcat(err, SDL_GetError());
        throw new FatalGameException(err);
    }
}

void Game::initRenderer() {
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

    Timer *capTimer = new Timer();
    Timer *fpsTimer = new Timer();
    fpsTimer->start();

    game->map->updateTexture();

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

        if (game->map->update)
            game->map->updateTexture();

        game->render();

        // Cap framerate
        int frameTicks = capTimer->getTicks();

        if (frameTicks < TICK_TIME)
            SDL_Delay((Uint32) TICK_TIME - frameTicks);

        game->frames++;
    }

    return 0;
}

void Game::main() {
    frames = 0;
    ticks = 0;
    Timer *capTimer = new Timer();

    SDL_Thread* renderThread = SDL_CreateThread(renderLoop, "RenderThread", (void *) this);

    const double UPDATE_RATE = 40;
    const double TICK_TIME = 1000 / UPDATE_RATE;

    while (running) {
        capTimer->start();
        event->handle();
        update();

        // Cap framerate
        int frameTicks = capTimer->getTicks();

        if (frameTicks < TICK_TIME)
            SDL_Delay((Uint32) TICK_TIME - frameTicks);

        ticks++;
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

    if (ticks % 100 == 0) {
        for (auto i = entities->begin(); i != entities->end(); i++) {
            if ((*i)->removable) {
                printf("Removing object.\n");
                entities->erase(i);
                delete *i++; // This makes sure we're no longer referencing the erased element
            }
        }
    }

    for (auto i = entities->begin(); i != entities->end(); i++) {
        (*i)->onUpdate();
    }
}

void Game::setup() {
    fpsTimer->start();
    event = new EventHandler(this);
    camera = new Camera(this);

    MapGenerator *generator = new MapGenerator(this);
    map = generator->generate();

    Team *team = new Team(this);
    team->setName("Cat Noises");

    team->setCatName(0, "Meow");
    team->setCatName(1, "Mew");
    team->setCatName(2, "Miauw");
    team->setCatName(3, "Nyan");
    team->setCatName(4, "Nya");

    vector<Cat*>* cats = team->generateCats();

    for (auto i = cats->begin(); i != cats->end(); i++) {
        map->addCat(*i);
        activeCat = *i;
        camera->target = *i;
    };



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
