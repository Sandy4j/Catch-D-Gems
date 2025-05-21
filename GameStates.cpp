#include "GameStates.h"
#include "GameManager.h"
#include "ObjectFactory.h"
#include "ScoreSystem.h"
#include "Player.h"
#include "InputHandler.h"
#include <algorithm>


void GameState::drawCenteredText(const std::string& text, float y, float fontSize, Color color) {
    GameManager* gm = GameManager::getInstance();
    Font font = gm->getFont();
    float textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
    DrawTextEx(font, text.c_str(),
        Vector2{ gm->getScreenWidth() / 2.0f - textWidth / 2, y },
        fontSize, 1, color);
}

void TitleState::enter() {
    GameManager* gm = GameManager::getInstance();
    gm->startBackgroundMusic();
}

void TitleState::update(float deltaTime) {
    GameManager* gm = GameManager::getInstance();
    InputHandler* input = gm->getInputHandler();

    if (input->isKeyPressed(KEY_ENTER)) {
        gm->changeState(new GameplayState());
    }
}

void TitleState::render() {
    GameManager* gm = GameManager::getInstance();
    ScoreSystem* scoreSystem = gm->getScoreSystem();

    drawCenteredText("COLLECT D'GEMS", gm->getScreenHeight() / 3, 40, SKYBLUE);
    drawCenteredText("Press ENTER to Start", gm->getScreenHeight() / 2, 20, LIGHTGRAY);
    drawCenteredText("Use LEFT and RIGHT arrows Or A and D to move", gm->getScreenHeight() / 2 + 40, 20, LIGHTGRAY);
    drawCenteredText("Avoid the Dynamites!", gm->getScreenHeight() / 2 + 70, 20, RED);

    if (scoreSystem->getHighScore() > 0) {
        drawCenteredText(TextFormat("High Score: %d", scoreSystem->getHighScore()),
            gm->getScreenHeight() / 2 + 120, 20, GOLD);
    }
}

void TitleState::exit() {
    GameManager* gm = GameManager::getInstance();
    gm->stopBackgroundMusic();
    gm->startBackgroundMusic();
}

void GameplayState::enter() {
    GameManager* gm = GameManager::getInstance();
    ScoreSystem* scoreSystem = gm->getScoreSystem();

    scoreSystem->resetScore();
    objects.clear();
    Player* player = gm->getPlayer();
    player->setPosition(Vector2{ gm->getScreenWidth() / 2.0f, player->getPosition().y });

    gm->resetSpawnTimer();
    if (!gm->isMusicPlaying()) {
        gm->startBackgroundMusic();
    }
}

void GameplayState::update(float deltaTime) {
    GameManager* gm = GameManager::getInstance();
    InputHandler* input = gm->getInputHandler();
    Player* player = gm->getPlayer();
    ScoreSystem* scoreSystem = gm->getScoreSystem();
    ObjectFactory* factory = gm->getObjectFactory();

    input->handleInput(player, deltaTime);
    player->update(deltaTime);
    scoreSystem->update(deltaTime);
    gm->updateSpawnTimer(deltaTime);
    if (gm->shouldSpawnObject()) {
        FallingObject* newObject = factory->createObject();
        addObject(newObject);
        gm->resetSpawnTimer();
    }

    for (auto object : objects) {
        if (object->isActive()) {
            object->update(deltaTime);
            if (object->checkCollision(player->getHitbox())) {

                if (object->getType() == ObjectType::DYNAMITE) {
                    gm->playExplosionSound();
                    gm->triggerScreenFlash(1.0f, RED);
                    player->setHit(true);
                    gm->changeState(new GameOverState());
                    object->setActive(false);
                    return;
                }
                else {
                    gm->playCollectSound();
                    scoreSystem->addScore(object->getScore(), object->getPosition(), object->getScoreColor());
                }
                object->setActive(false);
            }
        }
    }
    cleanupInactiveObjects();
}

void GameplayState::render() {
    GameManager* gm = GameManager::getInstance();
    ScoreSystem* scoreSystem = gm->getScoreSystem();
    Player* player = gm->getPlayer();

    int trackY = gm->getScreenHeight() - gm->getRailMid().height;
    int x = 0;
    DrawTexture(gm->getRailLeft(), x, trackY, WHITE);
    x += gm->getRailLeft().width;
    while (x + gm->getRailRight().width < gm->getScreenWidth()) {
        DrawTexture(gm->getRailMid(), x, trackY, WHITE);
        x += gm->getRailMid().width;
    }
    DrawTexture(gm->getRailRight(), gm->getScreenWidth() - gm->getRailRight().width, trackY, WHITE);

    for (auto object : objects) {
        if (object->isActive()) {
            object->render();
        }
    }
    player->render();
    scoreSystem->render();
}

void GameplayState::exit() {
    for (auto object : objects) {
        delete object;
    }
    objects.clear();
}

void GameplayState::addObject(FallingObject* object) {
    objects.push_back(object);
}

void GameplayState::cleanupInactiveObjects() {
    for (auto it = objects.begin(); it != objects.end();) {
        if (!(*it)->isActive()) {
            delete* it;
            it = objects.erase(it);
        }
        else {
            ++it;
        }
    }
}

void GameOverState::enter() {
    GameManager* gm = GameManager::getInstance();
    gm->stopBackgroundMusic();
}

void GameOverState::update(float deltaTime) {
    GameManager* gm = GameManager::getInstance();
    InputHandler* input = gm->getInputHandler();

    if (input->isKeyPressed(KEY_ENTER)) {
        gm->changeState(new GameplayState());
    }
}

void GameOverState::render() {
    GameManager* gm = GameManager::getInstance();
    ScoreSystem* scoreSystem = gm->getScoreSystem();

    drawCenteredText("GAME OVER!", gm->getScreenHeight() / 3, 40, RED);
    drawCenteredText(TextFormat("Final Score: %d", scoreSystem->getScore()),
        gm->getScreenHeight() / 2 - 20, 30, BLACK);

    if (scoreSystem->getScore() >= scoreSystem->getHighScore()) {
        drawCenteredText("NEW HIGH SCORE!", gm->getScreenHeight() / 2 + 20, 20, GOLD);
    }
    drawCenteredText("Press ENTER to Play Again", gm->getScreenHeight() / 2 + 60, 20, DARKGRAY);
    drawCenteredText("Press ESC to Exit", gm->getScreenHeight() / 2 + 90, 20, DARKGRAY);
}

void GameOverState::exit() {
}