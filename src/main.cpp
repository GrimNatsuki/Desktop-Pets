#define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>
#include "VectorStructs.hpp"
#include "PetEngine.hpp"

int displayID;
SDL_Rect displayBounds;

/*
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Surface *surface = NULL; 
static SDL_Texture *tex = NULL;


static Vector2int windowPos;
static Vector2f virtualPos;
static int windowWidth;
static int windowHeight;
static SDL_FRect srcRect;
static SDL_FRect dsRect;
*/

//PetState state = PetState::falling;


PetEngine Pet;
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("DesktopPetEngine", "1.0", "com.grimnatsuki.DesktopPets");
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
    /*
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, tex, &srcRect, &dsRect);
    SDL_RenderPresent(renderer);
    */
    Pet.displayWindow();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /*
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("Pet Engine quit");
    */

    Pet.destroyWindow();
}
