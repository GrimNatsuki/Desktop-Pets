class Timer
{
    Uint64 currentTime;
    Uint64 lastTime = 0;
    int timeCounterSeconds = 0;

    public:

    int elapsedSeconds()
    {
        currentTime = SDL_GetTicks();
        if (currentTime > lastTime + 1000)
        {
            timeCounterSeconds += 1;
            lastTime = currentTime;
            
        }
        return timeCounterSeconds;
    }

    Uint64 elapsedMilliSeconds()
    {
        return SDL_GetTicks();
    }

};