#pragma once
#include "raylib.h"

class GameManager;

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

#include "GameManager.h"

inline Player::Player(Vector2 startPos, float moveSpeed, float playerSize) :
    position(startPos),
    speed(moveSpeed),
    size(playerSize),
    hit(false),
    hitTimer(0.0f)
{
    texture = LoadTexture("Resources/Cart.png");
    hitbox = Rectangle{
        position.x - size / 2,
        position.y - size / 2,
        size,
        size
    };
}

inline Player::~Player() {
    UnloadTexture(texture);
}

inline void Player::update(float deltaTime) {
    if (hit) {
        hitTimer -= deltaTime;
        if (hitTimer <= 0.0f) {
            hit = false;
        }
    }
    hitbox.x = position.x - size / 2;
    hitbox.y = position.y - size / 2;
}

inline void Player::render() const {
    Color playerColor = hit ?
        Color{ 255, (unsigned char)(80 * (sinf(hitTimer * 30) * 0.5f + 0.5f)),
              (unsigned char)(80 * (sinf(hitTimer * 30) * 0.5f + 0.5f)), 255 } :
        WHITE;

    DrawTexture(
        texture,
        position.x - texture.width / 2,
        position.y - texture.height / 2,
        playerColor
    );
}

inline void Player::moveLeft(float deltaTime) {
    position.x -= speed * 60 * deltaTime;
    if (position.x < size / 2) {
        position.x = size / 2;
    }
}

inline void Player::moveRight(float deltaTime) {
    position.x += speed * 60 * deltaTime;
    GameManager* gm = GameManager::getInstance();
    if (position.x > gm->getScreenWidth() - size / 2) {
        position.x = gm->getScreenWidth() - size / 2;
    }
}

inline void Player::setPosition(Vector2 newPos) {
    position = newPos;
    hitbox.x = position.x - size / 2;
    hitbox.y = position.y - size / 2;
}