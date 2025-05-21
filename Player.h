#pragma once
#include "raylib.h"

class Player {
private:
    Vector2 position;
    float speed;
    Texture2D texture;
    float size;
    Rectangle hitbox;
    bool hit;
    float hitTimer;

public:
    Player(Vector2 startPos, float moveSpeed, float playerSize);
    ~Player();

    void update(float deltaTime);
    void render() const;

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    Vector2 getPosition() const { return position; }
    Rectangle getHitbox() const { return hitbox; }
    bool isHit() const { return hit; }

    void setPosition(Vector2 newPos);
    void setHit(bool isHit) { hit = isHit; hitTimer = isHit ? 0.5f : 0.0f; }
};