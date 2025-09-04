#include <random>
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


size_t jsonFileSize = 0;
void* jsonFileStream = nullptr;
std::string jsonText;
nlohmann::json config;
void PetEngine::loadConfig()
{
    try
    {
        jsonFileStream = SDL_LoadFile("conf/texConfig.json", &jsonFileSize);
        jsonText.assign(static_cast<char*> (jsonFileStream), jsonFileSize);
        SDL_free(jsonFileStream);
        config = nlohmann::json::parse(jsonText);
        properties.name = config["name"];
        properties.version = config["version"];
        properties.spriteFilePath = config["sprite_file_path"];
        properties.spriteSize = {config["sprite_size"][0], config["sprite_size"][1]};
        properties.displaySize = {config["display_size"][0], config["display_size"][1]};
        windowSize = properties.displaySize;
    }
    catch (const std::exception& e)
    {
        SDL_Log("Config reload failed: %s", e.what());
    }

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

void PetEngine::setPosition(float xPos, float yPos)
{
    virtualPos.x = xPos;
    virtualPos.y = yPos;
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

void PetEngine::faceRight(bool facingRight)
{
    this->facingRight = facingRight;
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
        case PetState::running:     SDL_Log("Pet is running"); break;
        case PetState::falling:     SDL_Log("Pet is falling"); break;
        case PetState::mousePicked: SDL_Log("Pet is picked up by mouse"); break;
        case PetState::floatUp:     SDL_Log("Pet is floating up"); break;
    }
}

Vector2int PetEngine::getPos()
{
    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);
    return windowPos;
}

void PetEngine::gravity()
{
    if (state == PetState::falling)
    {
        velocity.y += 0.5;
    }
    else if (state == PetState::idle)
    {
        velocity.y = 0;
    }
}

void PetEngine::fall()
{
    virtualPos.y += velocity.y;
}

void PetEngine::floatUp()
{
    virtualPos.y -= 4;
}

void PetEngine::walk()
{
    if (facingRight)
    {
        virtualPos.x += velocity.x;
    }
    else
    {
        virtualPos.x -= velocity.x;
    }

}
void PetEngine::run()
{
    if (facingRight)
    {
        virtualPos.x += velocity.x * 3;
    }
    else
    {
        virtualPos.x -= velocity.x * 3;
    }

}

static std::random_device rd;
static std::mt19937 gen(rd());
static std::discrete_distribution<int> randInt {10, 60, 30};
static std::bernoulli_distribution randBool(0.5);

void PetEngine::randomSwitchState()
{
    int randomInt = randInt(gen);
    if ((state != PetState::falling) && (state != PetState::mousePicked) && (state != PetState::floatUp))
    {
        switch(randomInt)
        {
            case 0:
                state = PetState::idle;
                break;
            case 1:
                if (state == PetState::idle) {state = PetState::walking;}
                else {state = PetState::idle;}
                break;
            case 2:
                if (state == PetState::idle) {state = PetState::running;}
                else {state = PetState::idle;}
        }
    }

}

void PetEngine::randomSwitchDirection()
{
    facingRight = randBool(gen);
}

Vector2int PetEngine::getDisplaySize()
{
    return properties.displaySize;
}