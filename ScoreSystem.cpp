#include "ScoreSystem.h"
#include "GameManager.h"

FloatingText::FloatingText(Vector2 pos, int val, Color col) :
    position(pos),
    value(val),
    color(col),
    alpha(1.0f),
    timer(1.5f)
{
    text = "+" + std::to_string(value);
}

void FloatingText::update(float deltaTime) {
    position.y -= 50.0f * deltaTime;
    timer -= deltaTime;
    if (timer <= 0.5f) {
        alpha = timer / 0.5f;
    }
}

void FloatingText::render(Font font) const {
    DrawTextEx(font, text.c_str(),
        Vector2{ position.x - MeasureTextEx(font, text.c_str(), 20, 1).x / 2, position.y },
        20, 1, ColorAlpha(color, alpha));
}

bool FloatingText::shouldRemove() const {
    return timer <= 0.0f;
}

ScoreSystem::ScoreSystem() :
    currentScore(0),
    highScore(0)
{
    // GameManager sudah mengatur font
}

ScoreSystem::~ScoreSystem() {
    // GameManager yang unload akan font
}

void ScoreSystem::addScore(int points, Vector2 position, Color color) {
    currentScore += points;
    if (currentScore > highScore) {
        highScore = currentScore;
    }

    floatingTexts.push_back(FloatingText(position, points, color));
    notifyObservers(currentScore, points, position, color);
}

void ScoreSystem::resetScore() {
    currentScore = 0;
    floatingTexts.clear();
}

void ScoreSystem::update(float deltaTime) {
    for (auto& text : floatingTexts) {
        text.update(deltaTime);
    }
    cleanupInactiveTexts();
}

void ScoreSystem::render() const {
    GameManager* gm = GameManager::getInstance();
    DrawTextEx(font, TextFormat("Score: %d", currentScore), Vector2{ 10, 10 }, 30, 1, BLACK);

    if (highScore > 0) {
        DrawTextEx(font, TextFormat("High Score: %d", highScore), Vector2{ 10, 50 }, 20, 1, DARKGRAY);
    }
    for (const auto& text : floatingTexts) {
        text.render(font);
    }
}

void ScoreSystem::cleanupInactiveTexts() {
    floatingTexts.erase(
        std::remove_if(
            floatingTexts.begin(),
            floatingTexts.end(),
            [](const FloatingText& text) { return text.shouldRemove(); }
        ),
        floatingTexts.end()
    );
}

void ScoreSystem::addObserver(ScoreObserver* observer) {
    observers.push_back(observer);
}

void ScoreSystem::removeObserver(ScoreObserver* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void ScoreSystem::notifyObservers(int score, int addedPoints, Vector2 position, Color color) {
    for (auto observer : observers) {
        observer->onScoreUpdate(score, addedPoints, position, color);
    }
}