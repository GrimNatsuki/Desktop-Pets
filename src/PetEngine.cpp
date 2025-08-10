#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "VectorStructs.hpp"

int PetEngine()
{
    static SDL_Window *window = NULL;
    static SDL_Renderer *renderer = NULL;
    static SDL_Surface *surface = NULL; 
    static SDL_Texture *tex = NULL;

    SDL_SetAppMetadata("DesktopPetEngine", "1.0", "com.grimnatsuki.PetEngine");
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    else
    {   
        SDL_Log("SDL initialized successfully");
    }

    static int windowWidth = 128;
    static int windowHeight = 128;

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "DesktopPetEngineWindow");
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

    SDL_FRect srcRect = {0, 0, 128, 128};
    SDL_FRect dsRect = {0, 0, 128, 128};

    SDL_Log("Pet Engine started");

    Vector2int windowPos;

    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);
    std::cout<<windowPos.x<<" "<<windowPos.y<<std::endl;  

    bool isRunning=true;

    bool isFalling=true;

    Vector2f virtualPos = {static_cast<float>(windowPos.x), static_cast<float>(windowPos.y)};

    while (isRunning)
    {
        //updates window position per frame

        windowPos.x = virtualPos.x;
        windowPos.y = virtualPos.y;

        SDL_SetWindowPosition(window, windowPos.x, windowPos.y);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
			{
				std::cout << "Pet Engine Terminated" << std::endl;
				isRunning = false;
            }
        }

        if (isFalling)
        {
            virtualPos.y += 0.1;
        }



        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    	SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, tex, &srcRect, &dsRect);
        
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("Pet Engine quit");

    std::cout<<"Closing app..."<<std::endl;

    return 0;
}

