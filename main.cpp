#include "raylib.h"
#include "GameManager.h"
#include "ScoreSystem.h"

class SoundObserver : public ScoreObserver {
private:
    GameManager* gameManager;

public:
    SoundObserver(GameManager* gm) : gameManager(gm) {}

    void onScoreUpdate(int score, int addedPoints, Vector2 position, Color color) override {
        gameManager->playCollectSound();
    }
};

int main() {
    GameManager* gameManager = GameManager::getInstance();
    gameManager->initialize();
    SoundObserver* soundObserver = new SoundObserver(gameManager);
    gameManager->getScoreSystem()->addObserver(soundObserver);
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        gameManager->update(deltaTime);
        gameManager->render();
    }
    delete soundObserver;
    gameManager->cleanup();

    return 0;
}