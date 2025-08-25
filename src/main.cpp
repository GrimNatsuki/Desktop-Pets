#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>
#include "VectorStructs.hpp"
#include "desktopPetsHeaders.hpp"


enum class PetState
{
    idle,
    walking,
    running,
    falling,
    mousePicked
    
};

struct PetProperties
{
    std::string name;
    std::string version;
    std::string imageFilePath;
    Vector2int spriteSize;
    Vector2int displaySize;
};

class PetEngine
{
    PetProperties Properties;
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Vector2int windowPos;
    Vector2f virtualPos;

    SDL_Surface *surface = NULL;
    SDL_Texture *tex = NULL;
    SDL_FRect windowSize;
    SDL_FRect srcRect;
    SDL_FRect dsRect;

    PetState state = PetState::idle;

};

static int displayID;
static SDL_Rect displayBounds;

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

PetState state = PetState::falling;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("DesktopPetEngine", "1.0", "com.grimnatsuki.DesktopPets");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetDisplays(&displayID);
    SDL_GetDisplayBounds(displayID, &displayBounds);

    windowWidth = 128;
    windowHeight = 128;

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "DesktopPetWindow");
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowWidth);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowHeight);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN, true);

    window = SDL_CreateWindowWithProperties(props);

    renderer = SDL_CreateRenderer(window, NULL);
    surface = IMG_Load("tex/CartethyiaSpriteMap.png");
    tex = SDL_CreateTextureFromSurface(renderer, surface);

    srcRect = {0, 0, 128, 128};
    dsRect = {0, 0, 128, 128};

    SDL_Log("Pet Engine started");
    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);

    virtualPos = {static_cast<float>(windowPos.x), static_cast<float>(windowPos.y)};

    /*
    size_t size = 0;
    void* jsonFileStream = SDL_LoadFile("conf/config.json", &size);
    std::string jsonText(static_cast<char*> (jsonFileStream), size);
    SDL_free(jsonFileStream);
    nlohmann::json config = nlohmann::json::parse(jsonText);
    std::string petName = config["name"];
    SDL_Log(petName.c_str());
    */

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
        windowPos.x = virtualPos.x;
        windowPos.y = virtualPos.y;

        SDL_SetWindowPosition(window, windowPos.x, windowPos.y);

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


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    	SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, tex, &srcRect, &dsRect);
        SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("Pet Engine quit");
}
