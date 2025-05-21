#include "Player.h"
#include "GameManager.h"

Player::Player(Vector2 startPos, float moveSpeed, float playerSize) :
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

Player::~Player() {
    UnloadTexture(texture);
}

void Player::update(float deltaTime) {
    if (hit) {
        hitTimer -= deltaTime;
        if (hitTimer <= 0.0f) {
            hit = false;
        }
    }
    hitbox.x = position.x - size / 2;
    hitbox.y = position.y - size / 2;
}

void Player::render() const {
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

void Player::moveLeft(float deltaTime) {
    position.x -= speed * 60 * deltaTime;
    if (position.x < size / 2) {
        position.x = size / 2;
    }
}

void Player::moveRight(float deltaTime) {
    position.x += speed * 60 * deltaTime;
    GameManager* gm = GameManager::getInstance();
    if (position.x > gm->getScreenWidth() - size / 2) {
        position.x = gm->getScreenWidth() - size / 2;
    }
}

void Player::setPosition(Vector2 newPos) {
    position = newPos;
    hitbox.x = position.x - size / 2;
    hitbox.y = position.y - size / 2;
}