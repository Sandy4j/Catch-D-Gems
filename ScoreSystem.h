#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>

class GameManager;

class ScoreObserver {
public:
    virtual ~ScoreObserver() {}
    virtual void onScoreUpdate(int score, int addedPoints, Vector2 position, Color color) = 0;
};

struct FloatingText {
    Vector2 position;
    std::string text;
    float alpha;
    float timer;
    int value;
    Color color;

    FloatingText(Vector2 pos, int val, Color col);
    void update(float deltaTime);
    void render(Font font) const;
    bool shouldRemove() const;
};

class ScoreSystem {
private:
    int currentScore;
    int highScore;
    std::vector<ScoreObserver*> observers;
    std::vector<FloatingText> floatingTexts;
    Font font;

public:
    ScoreSystem();
    ~ScoreSystem();

    void addScore(int points, Vector2 position, Color color);
    void resetScore();
    void update(float deltaTime);
    void render() const;
    void cleanupInactiveTexts();

    void addObserver(ScoreObserver* observer);
    void removeObserver(ScoreObserver* observer);
    void notifyObservers(int score, int addedPoints, Vector2 position, Color color);

    int getScore() const { return currentScore; }
    int getHighScore() const { return highScore; }
    void setFont(Font newFont) { font = newFont; }
};

#include "GameManager.h"

inline FloatingText::FloatingText(Vector2 pos, int val, Color col) :
    position(pos),
    value(val),
    color(col),
    alpha(1.0f),
    timer(1.5f)
{
    text = "+" + std::to_string(value);
}

inline void FloatingText::update(float deltaTime) {
    position.y -= 50.0f * deltaTime;
    timer -= deltaTime;
    if (timer <= 0.5f) {
        alpha = timer / 0.5f;
    }
}

inline void FloatingText::render(Font font) const {
    DrawTextEx(font, text.c_str(),
        Vector2{ position.x - MeasureTextEx(font, text.c_str(), 20, 1).x / 2, position.y },
        20, 1, ColorAlpha(color, alpha));
}

inline bool FloatingText::shouldRemove() const {
    return timer <= 0.0f;
}

inline ScoreSystem::ScoreSystem() :
    currentScore(0),
    highScore(0)
{
    // GameManager will set the font
}

inline ScoreSystem::~ScoreSystem() {
    // GameManager will unload the font
}

inline void ScoreSystem::addScore(int points, Vector2 position, Color color) {
    currentScore += points;
    if (currentScore > highScore) {
        highScore = currentScore;
    }

    floatingTexts.push_back(FloatingText(position, points, color));
    notifyObservers(currentScore, points, position, color);
}

inline void ScoreSystem::resetScore() {
    currentScore = 0;
    floatingTexts.clear();
}

inline void ScoreSystem::update(float deltaTime) {
    for (auto& text : floatingTexts) {
        text.update(deltaTime);
    }
    cleanupInactiveTexts();
}

inline void ScoreSystem::render() const {
    GameManager* gm = GameManager::getInstance();
    DrawTextEx(font, TextFormat("Score: %d", currentScore), Vector2{ 10, 10 }, 30, 1, WHITE);

    if (highScore > 0) {
        DrawTextEx(font, TextFormat("High Score: %d", highScore), Vector2{ 10, 50 }, 20, 1, LIGHTGRAY);
    }
    for (const auto& text : floatingTexts) {
        text.render(font);
    }
}

inline void ScoreSystem::cleanupInactiveTexts() {
    floatingTexts.erase(
        std::remove_if(
            floatingTexts.begin(),
            floatingTexts.end(),
            [](const FloatingText& text) { return text.shouldRemove(); }
        ),
        floatingTexts.end()
    );
}

inline void ScoreSystem::addObserver(ScoreObserver* observer) {
    observers.push_back(observer);
}

inline void ScoreSystem::removeObserver(ScoreObserver* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

inline void ScoreSystem::notifyObservers(int score, int addedPoints, Vector2 position, Color color) {
    for (auto observer : observers) {
        observer->onScoreUpdate(score, addedPoints, position, color);
    }
}