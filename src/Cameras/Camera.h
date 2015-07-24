//
// Created by Fiskie on 26/05/15.
//

#ifndef rocketcats_CAMERA_H
#define rocketcats_CAMERA_H

#include "../Game.h"
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

#define ALIGNMENT_LEFT 0
#define ALIGNMENT_RIGHT 1
#define ALIGNMENT_CENTER 2

using namespace std;

class Game;

class Camera {
protected:
    Game *game;

    Camera();

    TTF_Font *font;
public:
    Camera(Game *game);

    virtual void render() = 0;

    void renderLabel(string label, int x, int y, int alignment);
    void renderLabel(string label, int x, int y);

    virtual ~Camera();
};

#endif //rocketcats_CAMERA_H
