#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "DesktopPetsApps.hpp"


int Launcher()
{

    static SDL_Window *window = NULL;
    static SDL_Renderer *renderer = NULL;

    SDL_SetAppMetadata("DesktopPetsLauncher", "0.1", "com.grimnatsuki.DesktopPets.Launcher");
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    else
    {   
        SDL_Log("SDL initialized successfully");
    }

    int displayID;
    SDL_GetDisplays(&displayID);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(displayID, &displayBounds);
    std::cout<<displayBounds.x<<", "<<displayBounds.y<<", "<<displayBounds.w<<", "<<displayBounds.h<<std::endl;

    static int windowWidth = 0.8*displayBounds.w; 
    static int windowHeight = 0.8*displayBounds.h;

    //TO DO: Enable tiling windows for Wayland, try SDL_CreateWindowWithProperties

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "DesktopPetsLauncher");
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowWidth);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowHeight);

    window = SDL_CreateWindowWithProperties(props);
    renderer = SDL_CreateRenderer(window, NULL);

    SDL_GetWindowProperties(window);

    SDL_FRect windowMarginBounds = {0.1f*windowWidth, 0.1f*windowHeight, 0.8f*windowWidth, 0.8f*windowHeight};

    //TO DO: Make Icons Resizable depending on windows size

    int rectCount = 16;
    SDL_FRect iconRects[rectCount];

    int iconPerRow = 4;
    int iconRows = rectCount/iconPerRow;
    float iconSize = 200.f;
    float iconSpacingX = windowWidth*0.2f;
    float iconSpacingY = windowHeight*0.4f;

    for (int i = 0; i<iconRows;i++)
    {   
        for (int j=0;j<iconPerRow;j++)
        {
            float iconRectXPos = j*iconSpacingX+(windowWidth*0.1f);
            float iconRectYPos = i*iconSpacingY+(windowHeight*0.1f);
            iconRects[(i*iconPerRow)+j] = {iconRectXPos, iconRectYPos, iconSize , iconSize};
        }

    }

    bool isRunning=true;
    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
			{
				std::cout << "Launcher Menu Terminated" << std::endl;
				isRunning = false;
            }
        }

        //TO DO: Update window size with SDL_GetWindowSize
        
        //TO DO: Update Icon size

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    	SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
        SDL_RenderRects(renderer, iconRects, rectCount);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &windowMarginBounds);

        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("Launcher Menu quit");

    PetEngine();

    return 0;
}