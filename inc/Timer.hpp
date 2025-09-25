#pragma once

enum class TimerState
{
    active,
    inactive
};

class Timer
{
    TimerState state = TimerState::inactive;
    Uint64 currentTime;
    Uint64 lastTime = 0;
    int timeCounterSeconds = 0;

    Uint64 lifeTime = 0;
    Uint64 timerEnd = 0;

    public:
    Uint64 getLifeTime();
    void updateLifeTime();
    void startTimerMilliseconds(Uint64 milliseconds);
    void updateState();
    TimerState getState();
    int elapsedSeconds();
    Uint64 elapsedMilliSeconds();

};