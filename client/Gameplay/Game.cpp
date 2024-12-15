#include "Game.hpp"
#include "../raylib/raylib.hh"
#include <algorithm>
#include <cstdlib>

Game::Game() : playerPosition({50.0f, 300.0f}), playerSpeed(300.0f), missileSpeed(400.0f),
               obstacleSpawnTimer(0.0f), obstacleSpawnInterval(2.0f), score(0) {}

void Game::run() {
    InitWindow(800, 600, "R-TYPE");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float frameTime = GetFrameTime();

        handleInput(frameTime);
        updateMissiles(frameTime);
        spawnObstacles(frameTime);
        checkCollisions();

        BeginDrawing();
        ClearBackground(BLACK);
        draw();
        EndDrawing();
    }

    CloseWindow();
}

void Game::handleInput(float frameTime) {
    if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed * frameTime;
    if (IsKeyDown(KEY_LEFT)) playerPosition.x -= playerSpeed * frameTime;
    if (IsKeyDown(KEY_UP)) playerPosition.y -= playerSpeed * frameTime;
    if (IsKeyDown(KEY_DOWN)) playerPosition.y += playerSpeed * frameTime;

    playerPosition.x = std::max(0.0f, std::min(750.0f, playerPosition.x));
    playerPosition.y = std::max(0.0f, std::min(550.0f, playerPosition.y));

    if (IsKeyPressed(KEY_SPACE)) {
        missiles.push_back({{playerPosition.x + 50.0f, playerPosition.y + 20.0f}, missileSpeed, true});
    }
}

void Game::updateMissiles(float frameTime) {
    for (auto &missile : missiles) {
        if (missile.active) {
            missile.position.x += missile.speed * frameTime;
            if (missile.position.x > 800) missile.active = false;
        }
    }

    missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [](const Missile &m) { return !m.active; }), missiles.end());
}

void Game::spawnObstacles(float frameTime) {
    obstacleSpawnTimer += frameTime;
    if (obstacleSpawnTimer >= obstacleSpawnInterval) {
        obstacles.push_back({{800.0f, static_cast<float>(rand() % 550)}, 3, true});
        obstacleSpawnTimer = 0.0f;
    }

    for (auto &obstacle : obstacles) {
        if (obstacle.active) {
            obstacle.position.x -= 100.0f * frameTime;
            if (obstacle.position.x < 0) obstacle.active = false;
        }
    }

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle &o) { return !o.active; }), obstacles.end());
}

void Game::checkCollisions() {
    for (auto &obstacle : obstacles) {
        if (!obstacle.active) continue;

        for (auto &missile : missiles) {
            if (!missile.active) continue;

            if (CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
                                   {obstacle.position.x, obstacle.position.y, 50, 50})) {
                missile.active = false;
                obstacle.hitsRemaining--;

                if (obstacle.hitsRemaining <= 0) {
                    obstacle.active = false;
                    score += 10;
                }
            }
        }
    }
}

void Game::draw() {
    DrawRectangle(playerPosition.x, playerPosition.y, 50, 50, DARKBLUE);

    for (const auto &missile : missiles) {
        if (missile.active) DrawRectangle(missile.position.x, missile.position.y, 20, 10, GOLD);
    }

    for (const auto &obstacle : obstacles) {
        if (obstacle.active) {
            DrawRectangle(obstacle.position.x, obstacle.position.y, 50, 50, RED);
            DrawText(TextFormat("%d", obstacle.hitsRemaining), obstacle.position.x + 15, obstacle.position.y + 15, 20, WHITE);
        }
    }

    DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    DrawText("Use arrows to move, SPACE to shoot", 10, 40, 20, LIGHTGRAY);
}
