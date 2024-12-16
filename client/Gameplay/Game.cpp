#include "Game.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Game::Game() : playerPosition({50.0f, 300.0f}), playerSpeed(300.0f), missileSpeed(400.0f),
    obstacleSpawnTimer(0.0f), obstacleSpawnInterval(2.0f), score(0), playerHealth(100),
    enemyHealth_(300), enemyPosition_({700.0f, 300.0f}), enemySpeed_(200.0f),
    enemyFireTimer_(0.0f), enemyFireInterval_(2.5f), gameOver(false) {}
 
void Game::run(rtype::RayLib &rl)
{
    rl.setTargetFPS(60);

    while (!rl.windowShouldClose()) {
        frameTime = rl.getFrameTime();

        if (!gameOver) {
            handleInput(rl);
            updateMissiles();
            updateEnemy();
            spawnObstacles();
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

void Game::handleInput(rtype::RayLib &rl)
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

void Game::updateMissiles()
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

    for (auto &missile : enemyMissiles_) {
        if (missile.active) {
            missile.position.x -= missile.speed * frameTime;
            if (missile.position.x < 0)
                missile.active = false;
        }
    }
    enemyMissiles_.erase(std::remove_if(enemyMissiles_.begin(),
    enemyMissiles_.end(), [](const Missile &m) { return !m.active; }), enemyMissiles_.end());
}

void Game::updateEnemy()
{
    static bool movingDown = true;

    if (movingDown) {
        enemyPosition_.y += enemySpeed_ * frameTime;
        if (enemyPosition_.y >= 550.0f)
            movingDown = false;
    } else {
        enemyPosition_.y -= enemySpeed_ * frameTime;
        if (enemyPosition_.y <= 0.0f)
            movingDown = true;
    }
    enemyFireTimer_ += frameTime;
    if (enemyFireTimer_ >= enemyFireInterval_) {
        enemyMissiles_.push_back({{enemyPosition_.x - 20.0f, enemyPosition_.y + 20.0f}, missileSpeed, true});
        enemyFireTimer_ = 0.0f;
    }
}

void Game::spawnObstacles()
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
        {enemyPosition_.x, enemyPosition_.y, 50, 50})) {
            missile.active = false;
            enemyHealth_ -= 10;

            if (enemyHealth_ <= 0)
                enemyHealth_ = 0;
        }
    }

    for (auto &missile : enemyMissiles_) {
        if (CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
        {playerPosition.x, playerPosition.y, 50, 50})) {
            missile.active = false;
            playerHealth -= 10;
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
            playerHealth -= 5;

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

    for (const auto &missile : enemyMissiles_)
        if (missile.active)
            rl.drawRectangle(missile.position.x, missile.position.y, 20, 10, RED);

    for (const auto &obstacle : obstacles) {
        if (obstacle.active) {
            rl.drawRectangle(obstacle.position.x, obstacle.position.y, 50, 50, RED);
            rl.drawText(TextFormat("%d", obstacle.hitsRemaining), obstacle.position.x + 15, obstacle.position.y + 15, 20, WHITE);
        }
    }
    rl.drawRectangle(enemyPosition_.x, enemyPosition_.y, 50, 50, PURPLE);
    rl.drawRectangle(enemyPosition_.x, enemyPosition_.y - 10, 50, 5, GRAY);
    rl.drawRectangle(enemyPosition_.x, enemyPosition_.y - 10, static_cast<int>(50 * (enemyHealth_ / 300.0f)), 5, RED);

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