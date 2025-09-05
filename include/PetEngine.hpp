#pragma once
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <vector>
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

    std::string exitButtonFilePath;
    Vector2int exitButtonSize;

    int spriteMapRows;
    int spriteMapColumns;

    std::vector<int> idleSpritesIndices;
    std::vector<int> walkingSpritesIndices;
    std::vector<int> runningSpritesIndices;
    std::vector<int> fallingSpritesIndices;
    std::vector<int> mousePickedSpritesIndices;
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
    SDL_FPoint center;

    SDL_Surface *buttonSurface = NULL;
    SDL_Texture *buttonTex = NULL;
    SDL_FRect buttonSrcRect;
    SDL_FRect buttonDsRect;

    std::vector<SDL_FRect> spriteRects;

    Vector2int windowPos;
    Vector2f virtualPos;

    PetState state = PetState::idle;

    
    Vector2f velocity = {1, 0};

    

    public:
    bool facingRight = true;
    bool showingExitButton = false;

    void createWindow();
    void loadConfig();
    void loadTexture();
    void displayWindow();

    void setSprite(int spriteIndex);
    void idleAnimate(float framesPerSecond);

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

    int getIdleSpriteIndex(int index);
    int getWalkingSpriteIndex(int index);
    int getRunningSpriteIndex(int index);
    int getFallingSpriteIndex(int index);
    int getMousePickedSpriteIndex(int index);

    int getSpriteIndex(int index);
    Vector2int getExitButtonSize();
};
