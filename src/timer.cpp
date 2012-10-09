#include "timer.h"

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;    
}

void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();    
}

void Timer::stop()
{
    started = false;
    paused = false;    
}

double Timer::getTimeInSeconds()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return (SDL_GetTicks() - startTicks)/1000;
        }    
    }
    return 0;    
}

void Timer::pause()
{
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;
        pausedTicks = (SDL_GetTicks() - startTicks)/1000;
    }
}

void Timer::unpause()
{
    if( paused == true )
    {
        paused = false;
        startTicks = (SDL_GetTicks() - pausedTicks)/1000;
        pausedTicks = 0;
    }
}

bool Timer::isStarted()
{
    return started;    
}

bool Timer::isPaused()
{
    return paused;    
}
