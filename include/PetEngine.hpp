#pragma once
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
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
    std::string spriteFilePath;
    Vector2int spriteSize;
    Vector2int displaySize;
};

class PetEngine
{
    PetProperties properties;


    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Vector2int windowSize;

    SDL_Surface *surface = NULL; 
    SDL_Texture *tex = NULL;
    SDL_FRect srcRect;
    SDL_FRect dsRect;

    Vector2int windowPos;
    Vector2f virtualPos;

    PetState state = PetState::idle;

    bool facingRight = true;

    public:
    void createWindow();
    void loadConfig();
    void loadTexture();
    void displayWindow();


    void updatePos();
    Vector2int getPos();

    void destroyWindow();
    void switchState(PetState state);
    void randomSwitchState();
    PetState getState();
    void logState();

    void fall();
    void walk();
    void run();
};
