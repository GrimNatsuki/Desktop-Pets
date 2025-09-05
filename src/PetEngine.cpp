#include <iostream>
#include <random>
#include <vector>
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
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
    window = SDL_CreateWindowWithProperties(props);
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_GetWindowPosition(window, &windowPos.x, &windowPos.y);
    virtualPos = {static_cast<float>(windowPos.x), static_cast<float>(windowPos.y)};
}


size_t jsonFileSize = 0;
void* jsonFileStream = nullptr;
std::string jsonText;
nlohmann::json petConfig;
void PetEngine::loadConfig()
{
    try
    {
        jsonFileStream = SDL_LoadFile("conf/petConfig.json", &jsonFileSize);
        jsonText.assign(static_cast<char*> (jsonFileStream), jsonFileSize);
        SDL_free(jsonFileStream);
        petConfig = nlohmann::json::parse(jsonText);
        properties.name = petConfig["name"];
        properties.version = petConfig["version"];
        properties.spriteFilePath = petConfig["sprite_file_path"];
        properties.spriteSize = {petConfig["sprite_size"][0], petConfig["sprite_size"][1]};
        properties.displaySize = {petConfig["display_size"][0], petConfig["display_size"][1]};
        properties.spriteMapRows = petConfig["sprite_map_rows"];
        properties.spriteMapColumns = petConfig["sprite_map_columns"];

        properties.exitButtonFilePath = petConfig["exit_button_file_path"];
        properties.exitButtonSize = {petConfig["exit_button_size"][0], petConfig["exit_button_size"][1]};

        for (int i = 0; i<petConfig["animation_sprite_index"]["idle_sprites"].size(); i++)
        {
            properties.idleSpritesIndices.push_back(petConfig["animation_sprite_index"]["idle_sprites"][i]);
        }
        for (int i = 0; i<petConfig["animation_sprite_index"]["walking_sprites"].size(); i++)
        {
            properties.walkingSpritesIndices.push_back(petConfig["animation_sprite_index"]["walking_sprites"][i]);
        }
        for (int i = 0; i<petConfig["animation_sprite_index"]["running_sprites"].size(); i++)
        {
            properties.runningSpritesIndices.push_back(petConfig["animation_sprite_index"]["running_sprites"][i]);
        }

        for (int i = 0; i<petConfig["animation_sprite_index"]["falling_sprites"].size(); i++)
        {
            properties.fallingSpritesIndices.push_back(petConfig["animation_sprite_index"]["falling_sprites"][i]);
        }
        for (int i = 0; i<petConfig["animation_sprite_index"]["mouse_picked_sprites"].size(); i++)
        {
            properties.mousePickedSpritesIndices.push_back(petConfig["animation_sprite_index"]["mouse_picked_sprites"][i]);
        }

        windowSize = properties.displaySize;
        center = {windowSize.x, 0};
        
    }
    catch (const std::exception& e)
    {
        SDL_Log("Config reload failed: %s", e.what());
    }
}

SDL_FRect tempRect;

void PetEngine::loadTexture()
{
    surface = IMG_Load(properties.spriteFilePath.c_str());
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    buttonSurface = IMG_Load(properties.exitButtonFilePath.c_str());
    buttonTex = SDL_CreateTextureFromSurface(renderer, buttonSurface);

    for (int i = 0; i<properties.spriteMapRows; i++)
    {
        for (int j = 0; j<properties.spriteMapColumns; j++)
        {
            tempRect = {j*properties.spriteSize.x, 
                        i*properties.spriteSize.y, 
                        properties.spriteSize.x, 
                        properties.spriteSize.y};
            spriteRects.push_back(tempRect);
        }
    }
    srcRect = spriteRects[0];
    dsRect = {0, 0, static_cast<float>(properties.displaySize.x), static_cast<float>(properties.displaySize.y)};

    buttonSrcRect = {0, 0, properties.exitButtonSize.x, properties.exitButtonSize.y};
    buttonDsRect = {properties.displaySize.x - properties.exitButtonSize.x, 0, properties.exitButtonSize.x, properties.exitButtonSize.y};

}

void PetEngine::setSprite(int spriteIndex)
{
    if (spriteIndex < 0 || spriteIndex > spriteRects.size() - 1)
    {
        SDL_Log("Unable to load sprite. Cause: index is out of range");
    }
    else
    {
        srcRect = spriteRects[spriteIndex];
    }
}

void PetEngine::displayWindow()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
    if (facingRight)
    {
        SDL_RenderTextureRotated(renderer, tex, &srcRect, &dsRect, 0, &center, SDL_FLIP_HORIZONTAL);
        if (showingExitButton)
        {
            SDL_RenderTextureRotated(renderer, buttonTex, &buttonSrcRect, &buttonDsRect, 0, &center, SDL_FLIP_HORIZONTAL);
    
        }
    }
    else
    {
        SDL_RenderTextureRotated(renderer, tex, &srcRect, &dsRect, 0, &center, SDL_FLIP_NONE);
        if (showingExitButton)
        {
            SDL_RenderTextureRotated(renderer, buttonTex, &buttonSrcRect, &buttonDsRect, 0, &center, SDL_FLIP_NONE);
        }
    }

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

int PetEngine::getIdleSpriteIndex(int index)
{
    index = index % properties.idleSpritesIndices.size();
    return properties.idleSpritesIndices[index];
}

int PetEngine::getWalkingSpriteIndex(int index)
{

    index = index % properties.walkingSpritesIndices.size();
    return properties.walkingSpritesIndices[index];
}

int PetEngine::getRunningSpriteIndex(int index)
{
    index = index % properties.runningSpritesIndices.size();
    return properties.runningSpritesIndices[index];
}

int PetEngine::getFallingSpriteIndex(int index)
{
    index = index % properties.fallingSpritesIndices.size();
    return properties.fallingSpritesIndices[index];
}

int PetEngine::getMousePickedSpriteIndex(int index)
{
    index = index % properties.mousePickedSpritesIndices.size();
    return properties.mousePickedSpritesIndices[index];
}

int PetEngine::getSpriteIndex(int index)
{
    switch (state)
    {
        case PetState::idle:
            index = index % properties.idleSpritesIndices.size();
            return properties.idleSpritesIndices[index];
            break;

        case PetState::walking:
            index = index % properties.walkingSpritesIndices.size();
            return properties.walkingSpritesIndices[index];
            break;

        case PetState::running:
            index = index % properties.runningSpritesIndices.size();
            return properties.runningSpritesIndices[index];
            break;

        case PetState::falling:
            index = index % properties.fallingSpritesIndices.size();
            return properties.fallingSpritesIndices[index];
            break;

        case PetState::floatUp:
            index = index % properties.idleSpritesIndices.size();
            return properties.idleSpritesIndices[index];
            break;
            
        case PetState::mousePicked:
            index = index % properties.mousePickedSpritesIndices.size();
            return properties.mousePickedSpritesIndices[index];
            break;
    }
}

Vector2int PetEngine::getExitButtonSize()
{
    return properties.exitButtonSize;
}

