#include <SDL3/SDL.h>
#include "Timer.hpp"

Uint64 Timer::getLifeTime()
{
    return lifeTime;
}

void Timer::updateLifeTime()
{
    lifeTime = SDL_GetTicks();
}

void Timer::startTimerMilliseconds(Uint64 milliseconds)
{
    timerEnd = lifeTime + milliseconds;
}

void Timer::updateState()
{
    if (lifeTime < timerEnd)
    {
        state = TimerState::active;
    }
    else 
    {
        state = TimerState::inactive;
    }
}

TimerState Timer::getState()
{
    return state;
}

int Timer::elapsedSeconds()
{
    currentTime = SDL_GetTicks();
    if (currentTime > lastTime + 1000)
    {
        timeCounterSeconds += 1;
        lastTime = currentTime;
        
    }
    return timeCounterSeconds;
}

Uint64 Timer::elapsedMilliSeconds()
{
    return SDL_GetTicks();
}