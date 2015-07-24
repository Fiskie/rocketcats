//
//  timer.h
//  test
//
//  Created by Fiskie on 22/05/2015.
//  Copyright (c) 2015 Fiskie. All rights reserved.
//

#ifndef __rocketcats__timer__
#define __rocketcats__timer__

#include <SDL2/SDL_timer.h>

class Timer {
private:
    int startTicks;
    int pausedTicks;
    bool paused;
    bool started;

public:
    Timer();

    void start();

    void stop();

    void pause();

    void unpause();

    int getTicks();

    bool isStarted();

    bool isPaused();
};

#endif //__rocketcats__timer__
