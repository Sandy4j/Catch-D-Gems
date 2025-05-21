#include "GameManager.h"
#include "Player.h"
#include "InputHandler.h"
#include "GameStates.h"
#include "ObjectFactory.h"
#include "ScoreSystem.h"
#include <cstdlib>
#include <ctime>

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() :
    currentState(nullptr),
    inputHandler(nullptr),
    scoreSystem(nullptr),
    objectFactory(nullptr),
    player(nullptr),
    screenFlash(false),
    flashAlpha(0.0f),
    flashTimer(0.0f),
    screenWidth(800),
    screenHeight(450),
    spawnTimer(0.0f),
    spawnInterval(1.0f)
{
}

GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::initialize() {
    InitWindow(screenWidth, screenHeight, "Collect D'Gems");
    InitAudioDevice();
    collectSound = LoadSound("Resources/collect.mp3");
    explodeSound = LoadSound("Resources/explode.mp3");
    bgm = LoadMusicStream("Resources/bgm.mp3");
    SetMusicVolume(bgm, 0.5f);

    srand(time(NULL));

    pixelFont = LoadFont("Resources/pixelated.ttf");
    background = LoadTexture("Resources/BG.png");
    railLeft = LoadTexture("Resources/RailLeft.png");
    railMid = LoadTexture("Resources/RailMid.png");
    railRight = LoadTexture("Resources/RailRight.png");

    inputHandler = new InputHandler();
    scoreSystem = new ScoreSystem();
    objectFactory = new ObjectFactory();

    int trackY = screenHeight - railMid.height;
    player = new Player(Vector2{ screenWidth / 2.0f, (float)trackY }, 5.0f, 50.0f);

    GameState* titleState = new TitleState();
    changeState(titleState);
    SetTargetFPS(60);
}

void GameManager::update(float deltaTime) {
    UpdateMusicStream(bgm);
    updateScreenFlash(deltaTime);
    updateSpawnTimer(deltaTime);

    if (currentState) {
        currentState->update(deltaTime);
    }
}

void GameManager::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);

    if (currentState) {
        currentState->render();
    }
    if (screenFlash) {
        DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(RED, flashAlpha));
    }

    EndDrawing();
}

void GameManager::cleanup() {
    delete currentState;
    delete inputHandler;
    delete scoreSystem;
    delete objectFactory;
    delete player;

    UnloadFont(pixelFont);
    UnloadTexture(background);
    UnloadTexture(railLeft);
    UnloadTexture(railMid);
    UnloadTexture(railRight);

    UnloadSound(collectSound);
    UnloadSound(explodeSound);
    UnloadMusicStream(bgm);

    CloseAudioDevice();
    CloseWindow();

    delete instance;
    instance = nullptr;
}

void GameManager::changeState(GameState* state) {
    if (currentState != nullptr) {
        delete currentState;
    }
    currentState = state;
    if (currentState != nullptr) {
        currentState->enter();
    }
}

void GameManager::triggerScreenFlash(float duration, Color color) {
    screenFlash = true;
    flashAlpha = 0.8f;
    flashTimer = duration;
}

void GameManager::updateScreenFlash(float deltaTime) {
    if (screenFlash) {
        flashTimer -= deltaTime;
        if (flashTimer <= 0.0f) {
            screenFlash = false;
            flashAlpha = 0.0f;
        }
        else {
            flashAlpha = 0.6f * (sinf(flashTimer * 10.0f) * 0.5f + 0.5f);
        }
    }
}

void GameManager::playCollectSound() {
    PlaySound(collectSound);
}

void GameManager::playExplosionSound() {
    PlaySound(explodeSound);
}

void GameManager::startBackgroundMusic() {
    if (!IsMusicStreamPlaying(bgm)) {
        PlayMusicStream(bgm);
    }
}

void GameManager::stopBackgroundMusic() {
    if (IsMusicStreamPlaying(bgm)) {
        StopMusicStream(bgm);
    }
}

bool GameManager::isMusicPlaying() const {
    return IsMusicStreamPlaying(bgm);
}

void GameManager::updateSpawnTimer(float deltaTime) {
    spawnTimer += deltaTime;
}

void GameManager::resetSpawnTimer() {
    spawnTimer = 0.0f;
    spawnInterval = GetRandomValue(50, 200) / 100.0f;
}

bool GameManager::shouldSpawnObject() const {
    return spawnTimer >= spawnInterval;
}