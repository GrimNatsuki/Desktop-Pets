#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "desktopPetsHeaders.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    SDL_SetAppMetadata("Desktop Pets", "1.0", "com.grimnatsuki.DesktopPets");

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("App1 started");
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
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    Launcher();
}
