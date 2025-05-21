#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class GameState;
class InputHandler;
class ScoreSystem;
class ObjectFactory;
class Player;

class GameManager {
private:
    GameManager();

    static GameManager* instance;

    GameState* currentState;
    InputHandler* inputHandler;
    ScoreSystem* scoreSystem;
    ObjectFactory* objectFactory;
    Player* player;

    Font pixelFont;
    Texture2D background;
    Texture2D railLeft, railMid, railRight;

    Music bgm;
    Sound collectSound;
    Sound explodeSound;

    bool screenFlash;
    float flashAlpha;
    float flashTimer;

    int screenWidth;
    int screenHeight;

    float spawnTimer;
    float spawnInterval;

public:
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    static GameManager* getInstance();

 
    void initialize();
    void update(float deltaTime);
    void render();

    void cleanup();

    void changeState(GameState* state);

    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    GameState* getCurrentState() const { return currentState; }
    InputHandler* getInputHandler() const { return inputHandler; }
    ScoreSystem* getScoreSystem() const { return scoreSystem; }
    ObjectFactory* getObjectFactory() const { return objectFactory; }
    Player* getPlayer() const { return player; }
    Font getFont() const { return pixelFont; }
    Texture2D getBackground() const { return background; }
    Texture2D getRailLeft() const { return railLeft; }
    Texture2D getRailMid() const { return railMid; }
    Texture2D getRailRight() const { return railRight; }

    void triggerScreenFlash(float duration, Color color);
    void updateScreenFlash(float deltaTime);
    bool isScreenFlashing() const { return screenFlash; }
    float getFlashAlpha() const { return flashAlpha; }

    void playCollectSound();
    void playExplosionSound();
    void startBackgroundMusic();
    void stopBackgroundMusic();
    bool isMusicPlaying() const;

    void updateSpawnTimer(float deltaTime);
    void resetSpawnTimer();
    bool shouldSpawnObject() const;
};