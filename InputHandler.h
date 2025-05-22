#pragma once
#include "raylib.h"

class Player;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Player* player, float deltaTime) = 0;
};

class MoveLeftCommand : public Command {
public:
    void execute(Player* player, float deltaTime) override;
};

class MoveRightCommand : public Command {
public:
    void execute(Player* player, float deltaTime) override;
};

class InputHandler {
private:
    Command* leftCommand;
    Command* rightCommand;

public:
    InputHandler() {
        leftCommand = new MoveLeftCommand();
        rightCommand = new MoveRightCommand();
    }

    ~InputHandler() {
        delete leftCommand;
        delete rightCommand;
    }

    void handleInput(Player* player, float deltaTime);
    bool isKeyPressed(int key) const {
        return IsKeyPressed(key);
    }
};

#include "Player.h"

inline void MoveLeftCommand::execute(Player* player, float deltaTime) {
    player->moveLeft(deltaTime);
}

inline void MoveRightCommand::execute(Player* player, float deltaTime) {
    player->moveRight(deltaTime);
}

inline void InputHandler::handleInput(Player* player, float deltaTime) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        leftCommand->execute(player, deltaTime);
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        rightCommand->execute(player, deltaTime);
    }
}