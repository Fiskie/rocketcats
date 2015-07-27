//
// Created by Fiskie on 26/05/15.
//

#ifndef rocketcats_CAMERA_H
#define rocketcats_CAMERA_H

#include "Game.h"
#include "World/Cat.h"
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

#define ALIGNMENT_LEFT 0
#define ALIGNMENT_RIGHT 1
#define ALIGNMENT_CENTER 2

#define TRACK_DISABLE 0
#define TRACK_LOOSE 1
#define TRACK_STRICT 2

using namespace std;

class Game;
class Cat;

class Camera {
private:
    int trackingMode = TRACK_STRICT;
    TTF_Font *font;
    AbsPos frameOrigin;
public:
    const AbsPos &getFrameOrigin() const {
        return frameOrigin;
    }

    Game *game;

    Camera(Game *game);

    void renderLabel(string label, int x, int y, int alignment);
    void renderLabel(string label, int x, int y);

    double zoom;
    AbsPos origin;
    Cat *target;

    AbsPos getOrigin();

    void render();
    void renderMap();
    void renderBg();

    void setTrackingMode(int trackingMode) {
        this->trackingMode = trackingMode;
    }

    int getTrackingMode() {
        return trackingMode;
    }

    void placeOriginAtActiveCat();

    virtual ~Camera();
};

#endif //rocketcats_CAMERA_H
