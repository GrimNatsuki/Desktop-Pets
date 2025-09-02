#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <string>
#include <random>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "VectorStructs.hpp"
#include "Timer.hpp"
#include "PetEngine.hpp"

int displayID;
SDL_Rect displayBounds;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::bernoulli_distribution randBool(0.5);
//call d(gen) to return a boolean
static std::uniform_int_distribution<int> randInt(1, 10000);
//call intRand(gen) to return a random int
static std::uniform_real_distribution<float> randFloat(0.0f, 1.0f);


Timer timer;
PetEngine Pet;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("DesktopPet", "1.0", "com.grimnatsuki.DesktopPets");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GetDisplays(&displayID);
    SDL_GetDisplayBounds(displayID, &displayBounds);


    Pet.loadConfig();
    Pet.createWindow();
    Pet.loadTexture();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) 
    {
        SDL_Log("App1 terminated by user");
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE; 
}


SDL_AppResult SDL_AppIterate(void *appstate)
{

    Pet.updatePos();
    timer.updateLifeTime();
    timer.updateState();
    if (timer.getState() == TimerState::inactive)
    {
        timer.startTimerMilliseconds((randInt(gen) % 5) * 1000);
        Pet.randomSwitchState();
        Pet.logState();
    }

    if (Pet.getState() == PetState::walking)
    {
        Pet.walk();
    }
    else if (Pet.getState() == PetState::running)
    {
        Pet.run();
    }

    //SDL_SetWindowPosition(window, windowPos.x, windowPos.y);
    /*
    if (virtualPos.y < displayBounds.h-windowHeight)
    {
        state = PetState::falling;
    }
    else
    {
        state = PetState::idle;
    }
    if (state == PetState::falling)
    {
        virtualPos.y += 0.1;
    }
    */
    Pet.displayWindow();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    std::cout<<timer.getLifeTime()<<std::endl;
    Pet.destroyWindow();
}
