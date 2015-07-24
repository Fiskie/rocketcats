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
#include "Event.h"
#include "Timer.h"
#include "World/Map.h"
#include "Cameras/Camera.h"
#include <list>
#include <string>

class Event;

class Map;

class Camera;

class Cat;

class Game {
private:
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    Event *event = NULL;
    Timer *fpsTimer = new Timer();
    bool running;
    Map *map;

// list<Camera> *cameras; todo
    Camera *camera;
    Cat *activeCat;
public:
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

    void update(double delta);

    void render();

    void setup();

    void initialize();

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
