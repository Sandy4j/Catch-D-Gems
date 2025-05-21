#pragma once
#include "raylib.h"
#include <string>

enum class ObjectType {
    DIAMOND,
    RUBY,
    AMETHYST,
    GOLDBAR,
    SILVERBAR,
    DYNAMITE,
    COUNT
};

class FallingObject {
private:
    Vector2 position;
    float speed;
    Texture2D texture;
    float size;
    bool active;
    ObjectType type;
    int score;
    float rotation;

public:
    FallingObject(Vector2 startPos, float fallingSpeed, Texture2D objTexture,
        float objSize, ObjectType objType, int scoreValue);

    void update(float deltaTime);
    void render() const;
    bool checkCollision(Rectangle other) const;
    bool isOffScreen() const;

    Vector2 getPosition() const { return position; }
    bool isActive() const { return active; }
    ObjectType getType() const { return type; }
    int getScore() const { return score; }
    Color getScoreColor() const;

    void setActive(bool isActive) { active = isActive; }
};

class ObjectFactory {
private:
    Texture2D objectTextures[static_cast<int>(ObjectType::COUNT)];

public:
    ObjectFactory();
    ~ObjectFactory();

    FallingObject* createObject();
    FallingObject* createObject(ObjectType type);
};