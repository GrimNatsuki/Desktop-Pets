#pragma once
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
enum class PetState
{
    idle,
    walking,
    running,
    falling,
    floatUp,
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
    Vector2f velocity = {1, 0};

    public:
    void createWindow();
    void loadConfig();
    void loadTexture();
    void displayWindow();


    void updatePos();
    Vector2int getPos();
    void setPosition(float xPos, float yPos);

    void destroyWindow();
    void switchState(PetState state);
    void faceRight(bool facingRight);
    void randomSwitchState();
    void randomSwitchDirection();
    PetState getState();
    void logState();

    void fall();
    void walk();
    void run();
    void gravity();
    void floatUp();

    Vector2int getDisplaySize();
};
