#include "InputHandler.h"

InputHandler::InputHandler() {
    leftCommand = new MoveLeftCommand();
    rightCommand = new MoveRightCommand();
}

InputHandler::~InputHandler() {
    delete leftCommand;
    delete rightCommand;
}

void InputHandler::handleInput(Player* player, float deltaTime) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        leftCommand->execute(player, deltaTime);
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        rightCommand->execute(player, deltaTime);
    }
}

bool InputHandler::isKeyPressed(int key) const {
    return IsKeyPressed(key);
}