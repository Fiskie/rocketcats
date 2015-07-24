//
// Created by Fiskie on 16/07/2015.
//

#ifndef ROCKETCATS_DEFAULTCAMERA_H
#define ROCKETCATS_DEFAULTCAMERA_H

#include "Camera.h"
#include "../Pos.h"

#define TRACK_DISABLE 0
#define TRACK_LOOSE 1
#define TRACK_STRICT 2

class DefaultCamera : public Camera {
private:
    int trackingMode = TRACK_STRICT;
public:
    double zoom;
    AbsPos origin;
    Cat *target;

    DefaultCamera(Game *game);
    AbsPos getOrigin();

    void render();
    void renderMap();
    void renderBg();
    void renderCat(Cat *cat);

    void setTrackingMode(int trackingMode) {
        this->trackingMode = trackingMode;
    }

    int getTrackingMode() {
        return trackingMode;
    }

    void placeOriginAtActiveCat();
};

#endif //ROCKETCATS_DEFAULTCAMERA_H
