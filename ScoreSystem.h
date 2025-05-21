#pragma once
#include "raylib.h"
#include <vector>
#include <string>

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