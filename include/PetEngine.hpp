#pragma once

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

    public:

    void loadConfig()
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

    void createWindow()
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

    void loadTexture()
    {
        surface = IMG_Load(properties.spriteFilePath.c_str());
        tex = SDL_CreateTextureFromSurface(renderer, surface);
        srcRect = {0, 0, static_cast<float>(properties.spriteSize.x), static_cast<float>(properties.spriteSize.y)};
        dsRect = {0, 0, static_cast<float>(properties.displaySize.x), static_cast<float>(properties.displaySize.y)};
    }

    void displayWindow()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    	SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, tex, &srcRect, &dsRect);
        SDL_RenderPresent(renderer);
    }

    void updatePos()
    {
        windowPos.x = virtualPos.x;
        windowPos.y = virtualPos.y;

        SDL_SetWindowPosition(window, windowPos.x, windowPos.y);
    }

    void destroyWindow()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Log("Pet Engine quit");
    }


};