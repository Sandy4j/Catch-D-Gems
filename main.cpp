#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// Struktur untuk objek yang berjalan
struct FallingObject {
    Vector2 position;
    float speed;
    Color color;
    float size;
    bool active;
};

// Fungsi untuk spawn objek baru
FallingObject SpawnObject(int screenWidth) {
    FallingObject obj;

    // Posisi random di sepanjang ujung atas window
    obj.position.x = GetRandomValue(20, screenWidth - 20);
    obj.position.y = -20.0f; // Mulai dari atas layar

    // Kecepatan random
    obj.speed = GetRandomValue(100, 300) / 100.0f;

    // Warna random
    obj.color = Color{
        static_cast<unsigned char>(GetRandomValue(50, 250)),
        static_cast<unsigned char>(GetRandomValue(50, 250)),
        static_cast<unsigned char>(GetRandomValue(50, 250)),
        255
    };

    // Ukuran random
    obj.size = GetRandomValue(20, 40);

    // Aktif
    obj.active = true;

    return obj;
}

int main() {
    // Inisialisasi window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Object Spawner - Raylib");

    // Set target FPS
    SetTargetFPS(60);

    // Seed untuk random number generator
    srand(time(NULL));

    // Vector untuk menyimpan objek
    std::vector<FallingObject> objects;

    // Timer untuk spawn objek baru
    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f; // Waktu dalam detik antara setiap spawn

    // Game loop
    while (!WindowShouldClose()) {
        // Update
        float deltaTime = GetFrameTime();

        // Update spawn timer
        spawnTimer += deltaTime;

        // Spawn objek baru jika waktu interval telah tercapai
        if (spawnTimer >= spawnInterval) {
            objects.push_back(SpawnObject(screenWidth));
            spawnTimer = 0.0f;

            // Reset interval dengan waktu random untuk spawn berikutnya
            spawnInterval = GetRandomValue(50, 200) / 100.0f;
        }

        // Update posisi semua objek
        for (auto& obj : objects) {
            if (obj.active) {
                obj.position.y += obj.speed * 60 * deltaTime; // 60 adalah faktor untuk konsistensi pada berbagai fps

                // Nonaktifkan objek jika keluar dari layar
                if (obj.position.y > screenHeight + 20) {
                    obj.active = false;
                }
            }
        }

        // Hapus objek yang tidak aktif dari vector
        objects.erase(
            std::remove_if(
                objects.begin(),
                objects.end(),
                [](const FallingObject& obj) { return !obj.active; }
            ),
            objects.end()
        );

        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render semua objek
        for (const auto& obj : objects) {
            DrawCircle(obj.position.x, obj.position.y, obj.size, obj.color);
        }

        // UI info
        DrawText(TextFormat("Active Objects: %d", (int)objects.size()), 10, 10, 20, BLACK);
        DrawText(TextFormat("Next Spawn: %.2f", spawnInterval - spawnTimer), 10, 40, 20, BLACK);

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}