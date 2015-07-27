//
//  game.h
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#ifndef __rocketcats__game__
#define __rocketcats__game__

#include <SDL2/sdl.h>
#include "EventHandler.h"
#include "Timer.h"
#include <list>
#include <string>

class EventHandler;

class Map;

class Camera;

class Cat;

class Game {
private:
    EventHandler *event = NULL;
    Timer *fpsTimer = new Timer();

// list<Camera> *cameras; todo
    Camera *camera;
    Cat *activeCat;
public:
    bool running;
    double delta;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Map *map;

    Camera *getCamera() const {
        return camera;
    }

    float avgFPS;

    Cat *getActiveCat() const {
        return activeCat;
    }

    int frames;

    ~Game();

    Game();

    void render();

    void setup();

    void initRenderer();
    void initLibraries();

    virtual void run();

    virtual void onExit();

    Map *getMap();

    void setMap(Map *map);

    SDL_Window *getWindow();

    SDL_Renderer *getRenderer();

    void stop();

    void setResolution(int x, int y);

    int originX;
    int originY;
    int resX;
    int resY;

    void cycleActive();

    void main();

    void update();
};

//missing string printf
//this is safe and convenient but not exactly efficient
inline std::string format(const char *fmt, ...) {
    int size = 512;
    char *buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if (size <= nsize) { //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize + 1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    return ret;
}

#endif //__rocketcats__game__
