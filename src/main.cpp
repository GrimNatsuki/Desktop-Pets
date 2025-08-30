#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "VectorStructs.hpp"
#include "PetEngine.hpp"

int displayID;
SDL_Rect displayBounds;

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

    

    if (Pet.getState() == PetState::idle)
    {
        SDL_Log("Pet is idle");
    }

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

    Pet.updateTime();
    Pet.updatePos();
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
    Pet.destroyWindow();
}
