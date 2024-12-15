#include "Game.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Game::Game() : playerPosition({50.0f, 300.0f}), playerSpeed(300.0f), missileSpeed(400.0f),
    obstacleSpawnTimer(0.0f), obstacleSpawnInterval(2.0f), score(0), playerHealth(100),
    enemyHealth(300), enemyPosition({700.0f, 300.0f}), enemySpeed(200.0f),
    enemyFireTimer(0.0f), enemyFireInterval(2.5f), gameOver(false) {}

void Game::run(rtype::RayLib &rl)
{
    rl.initWindow(800, 600, "R-TYPE");
    rl.setTargetFPS(60);

    while (!rl.windowShouldClose()) {
        float frameTime = rl.getFrameTime();

        if (!gameOver) {
            handleInput(rl, frameTime);
            updateMissiles(frameTime);
            updateEnemy(frameTime);
            spawnObstacles(frameTime);
            checkCollisions();
        }
        rl.beginDrawing();
        rl.clearBackground(BLACK);

        if (gameOver)
            drawGameOver(rl);
        else
            draw(rl);

        rl.endDrawing();
    }
    rl.closeWindow();
}

void Game::handleInput(rtype::RayLib &rl, float frameTime)
{
    if (rl.isKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed * frameTime;
    if (rl.isKeyDown(KEY_LEFT)) playerPosition.x -= playerSpeed * frameTime;
    if (rl.isKeyDown(KEY_UP)) playerPosition.y -= playerSpeed * frameTime;
    if (rl.isKeyDown(KEY_DOWN)) playerPosition.y += playerSpeed * frameTime;

    playerPosition.x = std::max(0.0f, std::min(750.0f, playerPosition.x));
    playerPosition.y = std::max(0.0f, std::min(550.0f, playerPosition.y));

    if (rl.isKeyPressed(KEY_SPACE))
        missiles.push_back({{playerPosition.x + 50.0f, playerPosition.y + 20.0f}, missileSpeed, true});
}

void Game::updateMissiles(float frameTime)
{
    for (auto &missile : missiles) {
        if (missile.active) {
            missile.position.x += missile.speed * frameTime;
            if (missile.position.x > 800)
                missile.active = false;
        }
    }
    missiles.erase(std::remove_if(missiles.begin(),
    missiles.end(), [](const Missile &m) { return !m.active; }), missiles.end());

    for (auto &missile : enemyMissiles) {
        if (missile.active) {
            missile.position.x -= missile.speed * frameTime;
            if (missile.position.x < 0)
                missile.active = false;
        }
    }
    enemyMissiles.erase(std::remove_if(enemyMissiles.begin(),
    enemyMissiles.end(), [](const Missile &m) { return !m.active; }), enemyMissiles.end());
}

void Game::updateEnemy(float frameTime)
{
    static bool movingDown = true;

    if (movingDown) {
        enemyPosition.y += enemySpeed * frameTime;
        if (enemyPosition.y >= 550.0f)
            movingDown = false;
    } else {
        enemyPosition.y -= enemySpeed * frameTime;
        if (enemyPosition.y <= 0.0f)
            movingDown = true;
    }
    enemyFireTimer += frameTime;
    if (enemyFireTimer >= enemyFireInterval) {
        enemyMissiles.push_back({{enemyPosition.x - 20.0f, enemyPosition.y + 20.0f}, missileSpeed, true});
        enemyFireTimer = 0.0f;
    }
}

void Game::spawnObstacles(float frameTime)
{
    obstacleSpawnTimer += frameTime;

    if (obstacleSpawnTimer >= obstacleSpawnInterval) {
        obstacles.push_back({{800.0f, static_cast<float>(rand() % 550)}, 3, true});
        obstacleSpawnTimer = 0.0f;
    }

    for (auto &obstacle : obstacles) {
        if (obstacle.active) {
            obstacle.position.x -= 100.0f * frameTime;
            if (obstacle.position.x < 0)
                obstacle.active = false;
        }
    }
    obstacles.erase(std::remove_if(obstacles.begin(),
    obstacles.end(), [](const Obstacle &o) { return !o.active; }), obstacles.end());
}

void Game::checkCollisions()
{
    // Collisions with missiles
    for (auto &obstacle : obstacles) {
        if (!obstacle.active)
            continue;

        for (auto &missile : missiles) {
            if (!missile.active)
                continue;

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

    for (auto &missile : missiles) {
        if (CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
        {enemyPosition.x, enemyPosition.y, 50, 50})) {
            missile.active = false;
            enemyHealth -= 50;

            if (enemyHealth <= 0)
                enemyHealth = 0;
        }
    }

    for (auto &missile : enemyMissiles) {
        if (CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
        {playerPosition.x, playerPosition.y, 50, 50})) {
            missile.active = false;
            playerHealth -= 20;
            if (playerHealth <= 0) {
                playerHealth = 0;
                gameOver = true;
            }
        }
    }

    for (auto &obstacle : obstacles) {
        if (obstacle.active && CheckCollisionRecs({playerPosition.x, playerPosition.y, 50, 50},
            {obstacle.position.x, obstacle.position.y, 50, 50})) {
            obstacle.active = false;
            playerHealth -= 10;

            if (playerHealth <= 0) {
                playerHealth = 0;
                gameOver = true;
            }
        }
    }
}

void Game::draw(rtype::RayLib &rl)
{
    rl.drawRectangle(playerPosition.x, playerPosition.y, 50, 50, DARKBLUE);
    rl.drawRectangle(playerPosition.x, playerPosition.y - 10, 50, 5, GRAY);
    rl.drawRectangle(playerPosition.x, playerPosition.y - 10, static_cast<int>(50 * (playerHealth / 100.0f)), 5, GREEN);

    for (const auto &missile : missiles)
        if (missile.active)
            rl.drawRectangle(missile.position.x, missile.position.y, 20, 10, GOLD);

    for (const auto &missile : enemyMissiles)
        if (missile.active)
            rl.drawRectangle(missile.position.x, missile.position.y, 20, 10, RED);

    for (const auto &obstacle : obstacles) {
        if (obstacle.active) {
            rl.drawRectangle(obstacle.position.x, obstacle.position.y, 50, 50, RED);
            rl.drawText(TextFormat("%d", obstacle.hitsRemaining), obstacle.position.x + 15, obstacle.position.y + 15, 20, WHITE);
        }
    }
    rl.drawRectangle(enemyPosition.x, enemyPosition.y, 50, 50, PURPLE);
    rl.drawRectangle(enemyPosition.x, enemyPosition.y - 10, 50, 5, GRAY);
    rl.drawRectangle(enemyPosition.x, enemyPosition.y - 10, static_cast<int>(50 * (enemyHealth / 300.0f)), 5, RED);

    rl.drawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    rl.drawText("Use arrows to move, SPACE to shoot", 10, 40, 20, LIGHTGRAY);
    rl.drawText(TextFormat("Health: %d", playerHealth), 10, 70, 20, RED);
}

void Game::drawGameOver(rtype::RayLib &rl)
{
    rl.drawText("GAME OVER", 250, 250, 50, RED);
    rl.drawText(TextFormat("Score: %d", score), 250, 310, 20, WHITE);
    rl.drawText("Press ESC to exit", 50, 550, 20, LIGHTGRAY);
}
