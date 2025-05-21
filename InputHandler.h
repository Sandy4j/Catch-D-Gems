#pragma once
#include "raylib.h"
#include "Player.h"

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Player* player, float deltaTime) = 0;
};

class MoveLeftCommand : public Command {
public:
    void execute(Player* player, float deltaTime) override {
        player->moveLeft(deltaTime);
    }
};

class MoveRightCommand : public Command {
public:
    void execute(Player* player, float deltaTime) override {
        player->moveRight(deltaTime);
    }
};

class InputHandler {
private:
    Command* leftCommand;
    Command* rightCommand;

public:
    InputHandler();
    ~InputHandler();
    void handleInput(Player* player, float deltaTime);

    bool isKeyPressed(int key) const;
};