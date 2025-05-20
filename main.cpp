#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>


enum GameState {
    TITLE,
    GAMEPLAY,
    GAMEOVER
};

struct FloatingText {
    Vector2 position;
    std::string text;
    float alpha;
    float timer;
    int value;
    Color color;
};

struct FallingObject {
    Vector2 position;
    float speed;
    Texture2D texture;
    float size;
    bool active;
    int type;
    int score;
    float rotation;
};

struct Player {
    Vector2 position;
    float speed;
    Texture2D texture;
    float size;
    Rectangle hitbox;
    bool hit;
    float hitTimer;
};


enum ObjectType {
    DIAMOND,
    RUBY,
    AMETHYST,
    GOLDBAR,
    SILVERBAR,
    DYNAMITE
};

FallingObject SpawnObject(int screenWidth, Texture2D textures[6]) {
    FallingObject obj;

    obj.position.x = GetRandomValue(20, screenWidth - 20);
    obj.position.y = -50.0f;
    obj.rotation = 0.0f;
    obj.speed = GetRandomValue(150, 350) / 100.0f;
    obj.size = 50.0f;

    int randomValue = GetRandomValue(1, 100);

    if (randomValue <= 5) {
        obj.type = DIAMOND;
        obj.score = 15;
    }
    else if (randomValue <= 13) {
        obj.type = RUBY;
        obj.score = 12;
    }
    else if (randomValue <= 25) {
        obj.type = AMETHYST;
        obj.score = 10;
    }
    else if (randomValue <= 50) {
        obj.type = GOLDBAR;
        obj.score = 8;
    }
    else if (randomValue <= 80) {
        obj.type = SILVERBAR;
        obj.score = 5;
    }
    else {
        obj.type = DYNAMITE;
        obj.score = 0;
    }
    obj.texture = textures[obj.type];
    obj.active = true;

    return obj;
}

FloatingText CreateFloatingText(Vector2 position, int value, Color color) {
    FloatingText ft;
    ft.position = position;
    ft.value = value;
    ft.text = "+" + std::to_string(value);
    ft.alpha = 1.0f;
    ft.timer = 1.5f;
    ft.color = color;
    return ft;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Treasure Collector Game");

    // Initialize audio
    InitAudioDevice();

    // Load sound effects
    Sound collectSound = LoadSound("Resources/collect.mp3");
    Sound explodeSound = LoadSound("Resources/explode.mp3");

    SetTargetFPS(60);

    srand(time(NULL));

    // Load custom font
    Font pixelFont = LoadFont("Resources/pixelated.ttf");

    Texture2D objectTextures[6];
    objectTextures[DIAMOND] = LoadTexture("Resources/Diamond.png");
    objectTextures[RUBY] = LoadTexture("Resources/Ruby.png");
    objectTextures[AMETHYST] = LoadTexture("Resources/Amethyst.png");
    objectTextures[GOLDBAR] = LoadTexture("Resources/Gold.png");
    objectTextures[SILVERBAR] = LoadTexture("Resources/Silver.png");
    objectTextures[DYNAMITE] = LoadTexture("Resources/Dynamite.png");


    Texture2D playerTexture = LoadTexture("Resources/Cart.png");
    Player player;
    player.position.x = screenWidth / 2.0f;
    player.position.y = screenHeight - 70.0f;
    player.speed = 5.0f;
    player.texture = playerTexture;
    player.size = 50.0f;
    player.hitbox = Rectangle{
        player.position.x - player.size / 2,
        player.position.y - player.size / 2,
        player.size,
        player.size
    };
    player.hit = false;
    player.hitTimer = 0.0f;

    GameState currentState = TITLE;
    int score = 0;
    int highScore = 0;

    // Floating text array
    std::vector<FloatingText> floatingTexts;

    std::vector<FallingObject> objects;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f;

    // Screen flash effect
    bool screenFlash = false;
    float flashAlpha = 0.0f;
    float flashTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        switch (currentState) {
        case TITLE: {
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = GAMEPLAY;
                score = 0;
                objects.clear();
                floatingTexts.clear();
                player.position.x = screenWidth / 2.0f;
                player.hit = false;
                screenFlash = false;
                flashAlpha = 0.0f;
            }
            break;
        }

        case GAMEPLAY: {
            // Update flash effect
            if (screenFlash) {
                flashTimer -= deltaTime;
                if (flashTimer <= 0.0f) {
                    screenFlash = false;
                    flashAlpha = 0.0f;
                }
                else {
                    // Pulsing effect
                    flashAlpha = 0.6f * (sinf(flashTimer * 10.0f) * 0.5f + 0.5f);
                }
            }

            // Update player hit animation
            if (player.hit) {
                player.hitTimer -= deltaTime;
                if (player.hitTimer <= 0.0f) {
                    player.hit = false;
                }
            }

            if (IsKeyDown(KEY_LEFT)) {
                player.position.x -= player.speed * 60 * deltaTime;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                player.position.x += player.speed * 60 * deltaTime;
            }
            if (player.position.x < player.size / 2) player.position.x = player.size / 2;
            if (player.position.x > screenWidth - player.size / 2) player.position.x = screenWidth - player.size / 2;
            player.hitbox.x = player.position.x - player.size / 2;
            player.hitbox.y = player.position.y - player.size / 2;
            spawnTimer += deltaTime;

            if (spawnTimer >= spawnInterval) {
                objects.push_back(SpawnObject(screenWidth, objectTextures));
                spawnTimer = 0.0f;
                spawnInterval = GetRandomValue(50, 200) / 100.0f;
            }

            // Update floating texts
            for (auto& ft : floatingTexts) {
                ft.position.y -= 50.0f * deltaTime; // Move upward
                ft.timer -= deltaTime;
                if (ft.timer <= 0.5f) {
                    ft.alpha = ft.timer / 0.5f; // Fade out during last 0.5 seconds
                }
            }

            // Remove expired floating texts
            floatingTexts.erase(
                std::remove_if(
                    floatingTexts.begin(),
                    floatingTexts.end(),
                    [](const FloatingText& ft) { return ft.timer <= 0.0f; }
                ),
                floatingTexts.end()
            );

            for (auto& obj : objects) {
                if (obj.active) {
                    obj.position.y += obj.speed * 60 * deltaTime;
                    obj.rotation += 90.0f * deltaTime;

                    Rectangle objRect = {
                        obj.position.x - obj.size / 2,
                        obj.position.y - obj.size / 2,
                        obj.size,
                        obj.size
                    };

                    if (CheckCollisionRecs(player.hitbox, objRect)) {
                        if (obj.type == DYNAMITE) {
                            // Play explosion sound
                            PlaySound(explodeSound);

                            // Red flash effect
                            screenFlash = true;
                            flashAlpha = 0.8f;
                            flashTimer = 1.0f;

                            // Player hit animation
                            player.hit = true;
                            player.hitTimer = 0.5f;

                            currentState = GAMEOVER;
                            if (score > highScore) {
                                highScore = score;
                            }
                        }
                        else {
                            // Play collect sound
                            PlaySound(collectSound);

                            score += obj.score;

                            // Create floating score text
                            Color textColor;
                            switch (obj.type) {
                            case DIAMOND: textColor = SKYBLUE; break;
                            case RUBY: textColor = RED; break;
                            case AMETHYST: textColor = PURPLE; break;
                            case GOLDBAR: textColor = GOLD; break;
                            case SILVERBAR: textColor = LIGHTGRAY; break;
                            default: textColor = WHITE;
                            }
                            floatingTexts.push_back(CreateFloatingText(obj.position, obj.score, textColor));
                        }
                        obj.active = false;
                    }

                    if (obj.position.y > screenHeight + 20) {
                        obj.active = false;
                    }
                }
            }
            objects.erase(
                std::remove_if(
                    objects.begin(),
                    objects.end(),
                    [](const FallingObject& obj) { return !obj.active; }
                ),
                objects.end()
            );
            break;
        }

        case GAMEOVER: {
            // Continue updating flash effect
            if (screenFlash) {
                flashTimer -= deltaTime;
                if (flashTimer <= 0.0f) {
                    screenFlash = false;
                    flashAlpha = 0.0f;
                }
                else {
                    flashAlpha = 0.6f * (sinf(flashTimer * 10.0f) * 0.5f + 0.5f);
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                currentState = GAMEPLAY;
                score = 0;
                objects.clear();
                floatingTexts.clear();
                player.position.x = screenWidth / 2.0f;
                player.hit = false;
                screenFlash = false;
                flashAlpha = 0.0f;
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                return 0;
            }
            break;
        }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentState) {
        case TITLE: {
            DrawTextEx(pixelFont, "TREASURE COLLECTOR",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "TREASURE COLLECTOR", 40, 1).x / 2,
                (float)screenHeight / 3 }, 40, 1, DARKBLUE);

            DrawTextEx(pixelFont, "Press ENTER to Start",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "Press ENTER to Start", 20, 1).x / 2,
                (float)screenHeight / 2 }, 20, 1, DARKGRAY);

            DrawTextEx(pixelFont, "Use LEFT and RIGHT arrows to move",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "Use LEFT and RIGHT arrows to move", 20, 1).x / 2,
                (float)screenHeight / 2 + 40 }, 20, 1, DARKGRAY);

            DrawTextEx(pixelFont, "Avoid the Dynamites!",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "Avoid the Dynamites!", 20, 1).x / 2,
                (float)screenHeight / 2 + 70 }, 20, 1, RED);

            if (highScore > 0) {
                DrawTextEx(pixelFont, TextFormat("High Score: %d", highScore),
                    Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, TextFormat("High Score: %d", highScore), 20, 1).x / 2,
                    (float)screenHeight / 2 + 120 }, 20, 1, GOLD);
            }
            break;
        }

        case GAMEPLAY: {
            for (const auto& obj : objects) {
                if (obj.active) {
                    DrawTexturePro(
                        obj.texture,
                        Rectangle{ 0, 0, (float)obj.texture.width, (float)obj.texture.height },
                        Rectangle{ obj.position.x, obj.position.y, obj.size, obj.size },
                        Vector2{ obj.size / 2, obj.size / 2 },
                        obj.rotation,
                        WHITE
                    );
                }
            }

            // Draw player with hit animation
            Color playerColor = player.hit ?
                Color{ 255, (unsigned char)(80 * (sinf(player.hitTimer * 30) * 0.5f + 0.5f)),
                      (unsigned char)(80 * (sinf(player.hitTimer * 30) * 0.5f + 0.5f)), 255 } :
                WHITE;

            DrawTexture(
                player.texture,
                player.position.x - player.texture.width / 2,
                player.position.y - player.texture.height / 2,
                playerColor
            );

            // Draw floating score text
            for (const auto& ft : floatingTexts) {
                DrawTextEx(pixelFont, ft.text.c_str(),
                    Vector2{ ft.position.x - MeasureTextEx(pixelFont, ft.text.c_str(), 20, 1).x / 2, ft.position.y },
                    20, 1, ColorAlpha(ft.color, ft.alpha));
            }

            DrawTextEx(pixelFont, TextFormat("Score: %d", score), Vector2{ 10, 10 }, 30, 1, BLACK);
            if (highScore > 0) {
                DrawTextEx(pixelFont, TextFormat("High Score: %d", highScore), Vector2{ 10, 50 }, 20, 1, DARKGRAY);
            }

            // Draw flash effect
            if (screenFlash) {
                DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(RED, flashAlpha));
            }

            break;
        }

        case GAMEOVER: {
            DrawTextEx(pixelFont, "GAME OVER!",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "GAME OVER!", 40, 1).x / 2,
                (float)screenHeight / 3 }, 40, 1, RED);

            DrawTextEx(pixelFont, TextFormat("Final Score: %d", score),
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, TextFormat("Final Score: %d", score), 30, 1).x / 2,
                (float)screenHeight / 2 - 20 }, 30, 1, BLACK);

            if (score >= highScore) {
                DrawTextEx(pixelFont, "NEW HIGH SCORE!",
                    Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "NEW HIGH SCORE!", 20, 1).x / 2,
                    (float)screenHeight / 2 + 20 }, 20, 1, GOLD);
            }

            DrawTextEx(pixelFont, "Press ENTER to Play Again",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "Press ENTER to Play Again", 20, 1).x / 2,
                (float)screenHeight / 2 + 60 }, 20, 1, DARKGRAY);

            DrawTextEx(pixelFont, "Press ESC to Exit",
                Vector2{ screenWidth / 2.0f - MeasureTextEx(pixelFont, "Press ESC to Exit", 20, 1).x / 2,
                (float)screenHeight / 2 + 90 }, 20, 1, DARKGRAY);

            // Draw flash effect
            if (screenFlash) {
                DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(RED, flashAlpha));
            }

            break;
        }
        }

        EndDrawing();
    }

    // Unload textures and sounds
    for (int i = 0; i < 6; i++) {
        UnloadTexture(objectTextures[i]);
    }
    UnloadTexture(playerTexture);
    UnloadFont(pixelFont);
    UnloadSound(collectSound);
    UnloadSound(explodeSound);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}