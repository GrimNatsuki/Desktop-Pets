#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <string>
#include <random>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "VectorStructs.hpp"
#include "Timer.hpp"
#include "PetEngine.hpp"

struct RectBounds
{
    int left;
    int right;
    int top;
    int down;
};

int displayID;
SDL_Rect displayBounds;
RectBounds marginBounds;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::bernoulli_distribution randBool(0.5);
//call d(gen) to return a boolean
static std::uniform_int_distribution<int> randInt(0, 10000);
//call intRand(gen) to return a random int
static std::uniform_real_distribution<float> randFloat(0.0f, 1.0f);


Timer timer;
Timer appTimer;
int appFramesPerSecond = 60;

Timer animationTimer;
int animationFramesPerSecond = 2;
int animationFramesCounter = 0;

PetEngine Pet;
Vector2f mousePos;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("DesktopPet", "1.0", "com.grimnatsuki.DesktopPets");
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GetDisplays(&displayID);
    SDL_GetDisplayBounds(displayID, &displayBounds);

    Pet.loadConfig();
    Pet.createWindow();
    Pet.loadTexture();

    marginBounds = {displayBounds.w/3, displayBounds.w*2/3, 0, (displayBounds.h-Pet.getDisplaySize().y)};

    std::cout<<Pet.getWalkingSpriteIndex(4)<<std::endl;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) 
    {
        //SDL_Log("App1 terminated by user");
        return SDL_APP_SUCCESS;
    }
    if (event->button.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        Pet.switchState(PetState::mousePicked);
        //Pet.logState();
        //std::cout<<"Mouse clicked"<<std::endl;
    }
    else if (event->button.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        Pet.switchState(PetState::idle);
        //std::cout<<"Mouse released"<<std::endl;
    }
    return SDL_APP_CONTINUE; 
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    timer.updateLifeTime();
    timer.updateState();
    appTimer.updateLifeTime();
    appTimer.updateState();
    animationTimer.updateLifeTime();
    animationTimer.updateState();
    SDL_GetGlobalMouseState(&mousePos.x, &mousePos.y);

    if (timer.getState() == TimerState::inactive && timer.getLifeTime() > 3000)
    {
        timer.startTimerMilliseconds((1 + (randInt(gen) % 4)) * 1000);
        Pet.randomSwitchState();
        //Pet.logState();
        if (Pet.getPos().x < marginBounds.left)
        {
            Pet.faceRight(true);
        }
        else if (Pet.getPos().x > marginBounds.right)
        {
            Pet.faceRight(false);
        }
        else
        {
            Pet.randomSwitchDirection();
        }
    }

    if (appTimer.getState() == TimerState::inactive)
    {
        appTimer.startTimerMilliseconds(1000/appFramesPerSecond);
        
        Pet.updatePos();

        Pet.gravity();
        if (Pet.getPos().y < marginBounds.down && Pet.getState() != PetState::mousePicked && Pet.getState() != PetState::floatUp)
        {
            Pet.switchState(PetState::falling);
        }
        else if (Pet.getState() == PetState::falling)
        {
            Pet.switchState(PetState::floatUp);
        }
        else if (Pet.getPos().y > marginBounds.down && Pet.getState() == PetState::floatUp)
        {
            Pet.floatUp();
        }
        else if (Pet.getState() == PetState::floatUp)
        {
            Pet.setPosition(Pet.getPos().x, marginBounds.down);
            Pet.switchState(PetState::idle);
        }
        switch (Pet.getState())
        {
            case PetState::idle:
                break;

            case PetState::walking:
                Pet.walk();
                break;

            case PetState::running:
                Pet.run();
                break;

            case PetState::falling:
                Pet.fall();
                break;

            case PetState::floatUp:
                break;

            case PetState::mousePicked:
                Pet.setPosition(mousePos.x - 64, mousePos.y - 8);
                break;
        }

        Pet.displayWindow();
    }

    if (animationTimer.getState() == TimerState::inactive)
    {
        animationTimer.startTimerMilliseconds(1000/animationFramesPerSecond);
        Pet.setSprite(Pet.getSpriteIndex(animationFramesCounter));
        animationFramesCounter++;
    }

    return SDL_APP_CONTINUE;

}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //std::cout<<timer.getLifeTime()<<std::endl;
    Pet.destroyWindow();
}
