#include "Game.h"
#include "Constants.h"


Game::Game()
    : window(nullptr),
      renderer(nullptr),
assetManager(),
tileMap(),
player(),
sign(640.0f,192.0f, "Welcome to Monster Quest!"),
dialogueManager(),
font(nullptr),
dialogueTextTexture(nullptr),
camera(WINDOW_WIDTH,WINDOW_HEIGHT),
running(true),
signInRange(false),
interactPressed(false),
interactKeyDown(false),
previousCounter(0),
event()
{
}

bool Game::Initialize() {

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL failed to initialize: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if (!TTF_Init()) {
        SDL_Log("SDL_ttf failed to initialize: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    //Create the window
    window = SDL_CreateWindow(
        "Monster Quest",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    //Create the renderer
    renderer = SDL_CreateRenderer(window,nullptr);

    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!SDL_SetRenderVSync(renderer,1)) {
        SDL_Log("Failed to enable VSync: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    // Load dialogue font
    font = TTF_OpenFont(
        "../assets/fonts/rubik_regular.ttf",
        24.0f);

    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    // Player walking texture initialization
    SDL_Texture* walkLeft =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_walk_left.bmp");
    SDL_Texture* walkDown =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_walk_down.bmp");
    SDL_Texture* walkUp =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_walk_up.bmp");
    SDL_Texture* walkRight =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_walk_right.bmp");

    // Player idle texture initialization
    SDL_Texture* idleLeft =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_idle_left.bmp");
    SDL_Texture* idleDown =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_idle_down.bmp");
    SDL_Texture* idleUp =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_idle_up.bmp");
    SDL_Texture* idleRight =
        assetManager.LoadTexture(renderer,
            "../assets/player/player_idle_right.bmp");


    if (!walkLeft ||
        !walkDown ||
        !walkUp ||
        !walkRight ||
        !idleLeft ||
        !idleDown ||
        !idleUp ||
        !idleRight) {

        Shutdown();
        return false;
    }
    player.SetAnimationTextures(
        walkLeft,
        walkDown,
        walkUp,
        walkRight,
        idleLeft,
        idleDown,
        idleUp,
        idleRight);

    SDL_Texture* signTexture =
        assetManager.LoadTexture(
            renderer,
            "../assets/objects/sign.bmp");

    if (!signTexture) {
        Shutdown();
        return false;
    }

    sign.SetTexture(signTexture);

    if (!tileMap.Initialize(renderer, assetManager)) {
        Shutdown();
        return false;
    }

    if (!tileMap.LoadMap("../assets/maps/map01.txt")) {
        Shutdown();
        return false;
    }

    return true;
}

void Game::ProcessInput() {

    interactPressed = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_E && !interactKeyDown) {
                interactKeyDown = true;
                interactPressed = true;
            }
        }
        if (event.type == SDL_EVENT_KEY_UP) {
            if (event.key.key == SDLK_E) {
                interactKeyDown = false;
            }
        }
    }
}

void Game::Update(float deltaTime) {

    player.Update(deltaTime, tileMap);

    signInRange = sign.IsInInteractionRange(player.GetRect());

    if (interactPressed) {
        //
        if (dialogueManager.IsActive()) {

            dialogueManager.AdvanceDialogue();

            if (dialogueManager.IsActive()) {

                CreateDialogueTextTexture();
            }
            else {

                if (dialogueTextTexture) {
                        SDL_DestroyTexture(dialogueTextTexture);
                        dialogueTextTexture = nullptr;
                    }
                }
            }
        //
        else if (signInRange) {
            dialogueManager.StartDialogue({
            "Welcome to Monster Quest!",
            "There is much to discover in this world.",
            "Your adventure is just beginning."
            });

            CreateDialogueTextTexture();
        }
    }

    camera.Update(player.GetRect(), tileMap);
}

void Game::CreateDialogueTextTexture() {

    if (dialogueTextTexture) {
        SDL_DestroyTexture(dialogueTextTexture);
        dialogueTextTexture = nullptr;
    }

    SDL_Color textColor{
    255,
    255,
    255,
    255
    };

    SDL_Surface* textSurface =
        TTF_RenderText_Blended(
            font,
            dialogueManager.GetCurrentText().c_str(),
            0,
            textColor);

    if (!textSurface) {
        SDL_Log(
            "Failed to render dialogue text: %s",
            SDL_GetError());
        return;
    }

    dialogueTextTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            textSurface);

    SDL_DestroySurface(textSurface);

    if (!dialogueTextTexture) {
        SDL_Log(
            "Failed to create dialogue text texture: %s",
            SDL_GetError());
    }
}

void Game::Render() {

    // Render pass
    SDL_SetRenderDrawColor(renderer, 40, 60, 100, 255);
    SDL_RenderClear(renderer);


    tileMap.Render(renderer, camera.GetX(), camera.GetY());

    sign.Render(renderer, camera.GetX(), camera.GetY());

    player.Render(renderer, camera.GetX(), camera.GetY());

    if (dialogueManager.IsActive()) {

        //Dialogue box background
        SDL_FRect dialogueBox{
            40.0f,
            WINDOW_HEIGHT - 150.0f,
            WINDOW_WIDTH - 80.0f,
            110.0f
        };

        SDL_SetRenderDrawColor(
            renderer,
            20,
            20,
            20,
            230);

        SDL_RenderFillRect(
            renderer,
            &dialogueBox);

        if (dialogueTextTexture) {
            float textWidth;
            float textHeight;

            SDL_GetTextureSize(
                dialogueTextTexture,
                &textWidth,
                &textHeight);

            SDL_FRect textRect{
            dialogueBox.x + 20.0f,
            dialogueBox.y + 20.0f,
            textWidth,
            textHeight
            };

            SDL_RenderTexture(
                renderer,
                dialogueTextTexture,
                nullptr,
                &textRect);
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::Run() {

    previousCounter = SDL_GetPerformanceCounter();

    while (running) {

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        // Make sure you divide by the frequency AFTER
        // subtracting the counters
        double deltaTime =
            static_cast<double>(currentCounter - previousCounter)/
                SDL_GetPerformanceFrequency();
        previousCounter = currentCounter;

        ProcessInput();

        // Update Function
        Update(static_cast<float>(deltaTime));

        // Render Function
        Render();

    }
}

void Game::Shutdown() {
    assetManager.Shutdown();

    if (dialogueTextTexture) {
        SDL_DestroyTexture(dialogueTextTexture);
        dialogueTextTexture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}