#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>
#include "VectorStructs.hpp"
#include "PetEngine.hpp"

void PetEngine::createWindow()
{
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "DesktopPetWindow");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, windowSize.x);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, windowSize.y);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
    window = SDL_CreateWindowWithProperties(props);
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);
    virtualPos = {static_cast<float>(windowPos.x), static_cast<float>(windowPos.y)};
}

void PetEngine::loadConfig()
{
    size_t size = 0;
    void* jsonFileStream = SDL_LoadFile("conf/config.json", &size);
    std::string jsonText(static_cast<char*> (jsonFileStream), size);
    SDL_free(jsonFileStream);
    nlohmann::json config = nlohmann::json::parse(jsonText);
    properties.name = config["name"];
    properties.version = config["version"];
    properties.spriteFilePath = config["sprite_file_path"];
    properties.spriteSize = {config["sprite_size"][0], config["sprite_size"][1]};
    properties.displaySize = {config["display_size"][0], config["display_size"][1]};
    windowSize = properties.displaySize;
}

void PetEngine::loadTexture()
{
    surface = IMG_Load(properties.spriteFilePath.c_str());
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    srcRect = {0, 0, static_cast<float>(properties.spriteSize.x), static_cast<float>(properties.spriteSize.y)};
    dsRect = {0, 0, static_cast<float>(properties.displaySize.x), static_cast<float>(properties.displaySize.y)};
}

void PetEngine::displayWindow()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, tex, &srcRect, &dsRect);
    SDL_RenderPresent(renderer);
}

void PetEngine::updatePos()
{
    windowPos.x = virtualPos.x;
    windowPos.y = virtualPos.y;
    SDL_SetWindowPosition(window, windowPos.x, windowPos.y);
}

void PetEngine::destroyWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("Pet Engine quit");
}

void PetEngine::switchState(PetState state)
{
    this->state = state;
}

PetState PetEngine::getState()
{
    return state;
}

void PetEngine::logState()
{
    switch (state)
    {
        case PetState::idle:        SDL_Log("Pet is idle"); break;
        case PetState::walking:     SDL_Log("Pet is walking"); break;
        case PetState::running:     SDL_Log("Pet is idle"); break;
        case PetState::falling:     SDL_Log("Pet is falling"); break;
        case PetState::mousePicked: SDL_Log("Pet is picked up by mouse"); break;
    }
}

Vector2int PetEngine::getPos()
{
    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);
    return windowPos;
}

void PetEngine::fall()
{
    virtualPos.y += 0.1;
}

void PetEngine::walk(Direction direction)
{
    switch (direction)
    {
        case Direction::right:
            virtualPos.x += 0.01;
            break;
        case Direction::left:
            virtualPos.x -= 0.01;
            break;
    }
}
