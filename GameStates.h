#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class GameState {
public:
    virtual ~GameState() {}

    virtual void enter() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void exit() = 0;

    void drawCenteredText(const std::string& text, float y, float fontSize, Color color);
};

class TitleState : public GameState {
public:
    void enter() override;
    void update(float deltaTime) override;
    void render() override;
    void exit() override;
};

class GameplayState : public GameState {
private:
    std::vector<class FallingObject*> objects;

public:
    void enter() override;
    void update(float deltaTime) override;
    void render() override;
    void exit() override;

    void addObject(class FallingObject* object);

    void cleanupInactiveObjects();
};

class GameOverState : public GameState {
public:
    void enter() override;
    void update(float deltaTime) override;
    void render() override;
    void exit() override;
};