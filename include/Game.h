#pragma once

#include "AssetManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Camera.h"
#include "Sign.h"
#include "NPC.h"
#include "DialogueManager.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>


class Game {
public:
    Game();
    bool Initialize();
    void Run();
    void Shutdown();



private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    AssetManager assetManager;
    TileMap tileMap;
    Player player;
    Sign sign;
    NPC rat;
    NPC eska;
    std::vector<NPC*> npcs;
    DialogueManager dialogueManager;
    TTF_Font* font;
    SDL_Texture* dialogueTextTexture;
    Camera camera;
    bool running;
    bool signInRange;
    bool interactPressed;
    bool interactKeyDown;
    Uint64 previousCounter;
    SDL_Event event;
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
    void CreateDialogueTextTexture();
};
