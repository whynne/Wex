#pragma once

#include "SDL/SDL.h"

class Timer
{
private:
    //The clock time when the timer started
    double startTicks;
    
    //The ticks stored when the timer was paused
    int pausedTicks;
    
    //The timer status
    bool paused;
    bool started;
    
public:
    //Initializes variables
    Timer();
    
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Gets the timer's time
    double getTimeInSeconds();
    
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();    
};
