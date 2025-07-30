#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include <chrono>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "DesktopPetsApps.hpp"

using timePoint = std::chrono::time_point<std::chrono::steady_clock>;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static timePoint spawnTime;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    spawnTime = std::chrono::steady_clock::now();  // Start time

    SDL_SetAppMetadata("Desktop Pets", "1.0", "com.grimnatsuki.DesktopPets");

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0, &window, &renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
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
    timePoint now = std::chrono::steady_clock::now();
    std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - spawnTime);

    if (elapsed.count() >= 2000) 
    {
        SDL_Log("2 seconds passed...");
        return SDL_APP_SUCCESS;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("App1 quit");

    Launcher();
}
