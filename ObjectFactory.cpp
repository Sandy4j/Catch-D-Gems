#include "ObjectFactory.h"
#include "GameManager.h"
#include <cstdlib>

FallingObject::FallingObject(Vector2 startPos, float fallingSpeed, Texture2D objTexture,
    float objSize, ObjectType objType, int scoreValue) :
    position(startPos),
    speed(fallingSpeed),
    texture(objTexture),
    size(objSize),
    active(true),
    type(objType),
    score(scoreValue),
    rotation(0.0f)
{
}

void FallingObject::update(float deltaTime) {
    position.y += speed * 60 * deltaTime;
    rotation += 90.0f * deltaTime;

    if (isOffScreen()) {
        active = false;
    }
}

void FallingObject::render() const {
    if (!active) return;

    DrawTexturePro(
        texture,
        Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
        Rectangle{ position.x, position.y, size, size },
        Vector2{ size / 2, size / 2 },
        rotation,
        WHITE
    );
}

bool FallingObject::checkCollision(Rectangle other) const {
    Rectangle objRect = {
        position.x - size / 2,
        position.y - size / 2,
        size,
        size
    };
    return CheckCollisionRecs(objRect, other);
}

bool FallingObject::isOffScreen() const {
    GameManager* gm = GameManager::getInstance();
    return position.y > gm->getScreenHeight() + size;
}

Color FallingObject::getScoreColor() const {
    switch (type) {
    case ObjectType::DIAMOND: return SKYBLUE;
    case ObjectType::RUBY: return RED;
    case ObjectType::AMETHYST: return PURPLE;
    case ObjectType::GOLDBAR: return GOLD;
    case ObjectType::SILVERBAR: return LIGHTGRAY;
    default: return WHITE;
    }
}

ObjectFactory::ObjectFactory() {
    objectTextures[static_cast<int>(ObjectType::DIAMOND)] = LoadTexture("Resources/Diamond.png");
    objectTextures[static_cast<int>(ObjectType::RUBY)] = LoadTexture("Resources/Ruby.png");
    objectTextures[static_cast<int>(ObjectType::AMETHYST)] = LoadTexture("Resources/Amethyst.png");
    objectTextures[static_cast<int>(ObjectType::GOLDBAR)] = LoadTexture("Resources/Gold.png");
    objectTextures[static_cast<int>(ObjectType::SILVERBAR)] = LoadTexture("Resources/Silver.png");
    objectTextures[static_cast<int>(ObjectType::DYNAMITE)] = LoadTexture("Resources/Dynamite.png");
}

ObjectFactory::~ObjectFactory() {
    for (int i = 0; i < static_cast<int>(ObjectType::COUNT); i++) {
        UnloadTexture(objectTextures[i]);
    }
}

FallingObject* ObjectFactory::createObject() {
    GameManager* gm = GameManager::getInstance();
    Vector2 startPos;
    startPos.x = GetRandomValue(20, gm->getScreenWidth() - 20);
    startPos.y = -50.0f;
    float speed = GetRandomValue(150, 350) / 100.0f;
    float size = 50.0f;

    int randomValue = GetRandomValue(1, 100);
    ObjectType type;
    int scoreValue;

    if (randomValue <= 5) {
        type = ObjectType::DIAMOND;
        scoreValue = 15;
    }
    else if (randomValue <= 13) {
        type = ObjectType::RUBY;
        scoreValue = 12;
    }
    else if (randomValue <= 25) {
        type = ObjectType::AMETHYST;
        scoreValue = 10;
    }
    else if (randomValue <= 50) {
        type = ObjectType::GOLDBAR;
        scoreValue = 8;
    }
    else if (randomValue <= 80) {
        type = ObjectType::SILVERBAR;
        scoreValue = 5;
    }
    else {
        type = ObjectType::DYNAMITE;
        scoreValue = 0;
    }

    return new FallingObject(startPos, speed, objectTextures[static_cast<int>(type)], size, type, scoreValue);
}

FallingObject* ObjectFactory::createObject(ObjectType type) {
    GameManager* gm = GameManager::getInstance();
    Vector2 startPos;
    startPos.x = GetRandomValue(20, gm->getScreenWidth() - 20);
    startPos.y = -50.0f;

    float speed = GetRandomValue(150, 350) / 100.0f;
    float size = 50.0f;
    int scoreValue;
    switch (type) {
    case ObjectType::DIAMOND: scoreValue = 15; break;
    case ObjectType::RUBY: scoreValue = 12; break;
    case ObjectType::AMETHYST: scoreValue = 10; break;
    case ObjectType::GOLDBAR: scoreValue = 8; break;
    case ObjectType::SILVERBAR: scoreValue = 5; break;
    case ObjectType::DYNAMITE: scoreValue = 0; break;
    default: scoreValue = 0;
    }

    return new FallingObject(startPos, speed, objectTextures[static_cast<int>(type)], size, type, scoreValue);
}