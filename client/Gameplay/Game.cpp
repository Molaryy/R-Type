#include "Game.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>

/**
 * @brief Construct a new Game:: Game object
 */
Game::Game() : playerPosition({50.0f, 300.0f}), playerSpeed(300.0f), missileSpeed(400.0f),
    asteroidSpawnTimer(0.0f), asteroidSpawnInterval(2.0f), score(0), playerHealth(100),
    enemyHealth_(300), enemyPosition_({700.0f, 300.0f}), enemySpeed_(200.0f),
    enemyFireTimer_(0.0f), enemyFireInterval_(2.5f), gameOver(false),
    isExploding(false), explosionTimer(0.0f), explosionSpeed(0.1f), explosionCurrentFrame(0),
    missileAnimationTimer(0.0f), enemyAnimationTimer(0.0f), animationSpeed(0.1f) {}

/**
 * @brief Game main function
 */
void Game::run(rtype::RayLib &rl)
{
    rl.setTargetFPS(60);
    // TODO: change by correct path
    playerSpriteSheet = rl.loadTexture("/Users/moonia/3/cpp/R-Type/assets/spaceship.gif");
    playerFrameRec = {0.0f, 0.0f, 33, 13};
    currentFrame = 0;
    animationTimer = 0.0f;
    // TODO: change by correct path
    missileSpriteSheet = rl.loadTexture("/Users/moonia/3/cpp/R-Type/assets/missiles.gif");
    missileFrameRec = {0.0f, 0.0f, 20, 10};
    // TODO: change by correct path
    explosionSpriteSheet = rl.loadTexture("/Users/moonia/3/cpp/R-Type/assets/damage.gif");
    explosionFrameRec = {0.0f, 0.0f, 64, 64};
    // TODO: change by correct path
    enemySpriteSheet = rl.loadTexture("/Users/moonia/3/cpp/R-Type/assets/enemies.gif");
    enemyFrameRec = {0.0f, 0.0f, 65, 50};

    while (!rl.windowShouldClose()) {
        frameTime = rl.getFrameTime();

        if (!gameOver || isExploding) {
            handleInput(rl);
            updateMissiles();
            updateEnemy();
            spawnAsteroids();
            checkCollisions();
            updateExplosion();
            updateAnimations();
        }
        rl.beginDrawing();
        rl.clearBackground(BLACK);

        if (gameOver && !isExploding)
            drawGameOver(rl);
        else
            draw(rl);
        rl.endDrawing();
    }
    rl.unloadTexture(playerSpriteSheet);
    rl.unloadTexture(missileSpriteSheet);
    rl.unloadTexture(explosionSpriteSheet);
    rl.unloadTexture(enemySpriteSheet);
    rl.closeWindow();
}

/**
 * @brief Handle input keys
 */
void Game::handleInput(rtype::RayLib &rl)
{
    if (isExploding)
        return;

    bool isMoving = false;
    if (rl.isKeyDown(KEY_RIGHT)) {
        playerPosition.x += playerSpeed * frameTime;
        isMoving = true;
    }
    if (rl.isKeyDown(KEY_LEFT)) {
        playerPosition.x -= playerSpeed * frameTime;
        isMoving = true;
    }
    if (rl.isKeyDown(KEY_UP)) {
        playerPosition.y -= playerSpeed * frameTime;
        isMoving = true;
    }
    if (rl.isKeyDown(KEY_DOWN)) {
        playerPosition.y += playerSpeed * frameTime;
        isMoving = true;
    }

    playerPosition.x = std::max(0.0f, std::min(750.0f, playerPosition.x));
    playerPosition.y = std::max(0.0f, std::min(550.0f, playerPosition.y));

    if (isMoving) {
        animationTimer += frameTime;
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.0f;
            currentFrame = (currentFrame + 1) % 2;
        }
    } else {
        currentFrame = 0;
    }
    playerFrameRec.x = currentFrame * playerFrameRec.width;

    if (rl.isKeyPressed(KEY_SPACE))
        missiles.push_back({{playerPosition.x + 50.0f, playerPosition.y + 20.0f}, missileSpeed, true});
}

/**
 * @brief Update animations for player, missiles, and enemies
 */
void Game::updateAnimations()
{
    missileAnimationTimer += frameTime;
    if (missileAnimationTimer >= animationSpeed) {
        missileAnimationTimer = 0.0f;
        missileFrameRec.x = (missileFrameRec.x == 0) ? missileFrameRec.width : 0;
    }

    enemyAnimationTimer += frameTime;
    if (enemyAnimationTimer >= animationSpeed) {
        enemyAnimationTimer = 0.0f;
        enemyFrameRec.x += enemyFrameRec.width;
        if (enemyFrameRec.x >= 5 * enemyFrameRec.width) {
            enemyFrameRec.x = 0;
        }
    }
}

/**
 * @brief Update explosion animation
 * 
 */
void Game::updateExplosion()
{
    if (isExploding) {
        explosionTimer += frameTime;

        if (explosionTimer >= explosionSpeed) {
            explosionTimer = 0.0f;
            explosionCurrentFrame++;

            if (explosionCurrentFrame >= 5) {
                isExploding = false;
                gameOver = true;
            } else {
                explosionFrameRec.x = explosionCurrentFrame * explosionFrameRec.width;
            }
        }
    }
}

/**
 * @brief Update player & enemy missiles positions 
 * 
 */
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

/**
 * @brief Update enemy position (up, down) & speed
 * 
 */
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


/**
 * @brief Spawn random asteroids on game
 * 
 */
void Game::spawnAsteroids()
{
    asteroidSpawnTimer += frameTime;

    if (asteroidSpawnTimer >= asteroidSpawnInterval) {
        asteroids.push_back({{800.0f, static_cast<float>(rand() % 550)}, 3, true});
        asteroidSpawnTimer = 0.0f;
    }

    for (auto &asteroid : asteroids) {
        if (asteroid.active) {
            asteroid.position.x -= 100.0f * frameTime;
            if (asteroid.position.x < 0)
                asteroid.active = false;
        }
    }
    asteroids.erase(std::remove_if(asteroids.begin(),
    asteroids.end(), [](const Asteroid &o) { return !o.active; }), asteroids.end());
}

/**
 * @brief Check if collisions, then update entities health bars
 * 
 */
void Game::checkCollisions()
{
    if (isExploding)
        return;

    for (auto &asteroid : asteroids) {
        if (!asteroid.active)
            continue;

        for (auto &missile : missiles) {
            if (missile.active && CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
            {asteroid.position.x, asteroid.position.y, 50, 50})) {
                missile.active = false;
                asteroid.hitsRemaining--;

                if (asteroid.hitsRemaining <= 0) {
                    asteroid.active = false;
                    score += 10;
                }
            }
        }
        if (CheckCollisionRecs({playerPosition.x, playerPosition.y, 50, 50},
        {asteroid.position.x, asteroid.position.y, 50, 50})) {
            playerHealth -= 10;
            asteroid.active = false;

            if (playerHealth <= 0) {
                playerHealth = 0;
                isExploding = true;
                explosionTimer = 0.0f;
                explosionCurrentFrame = 0;
            }
        }
    }

    for (auto &missile : enemyMissiles_) {
        if (missile.active && CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
        {playerPosition.x, playerPosition.y, 50, 50})) {
            missile.active = false;
            playerHealth -= 10;

            if (playerHealth <= 0) {
                playerHealth = 0;
                isExploding = true;
                explosionTimer = 0.0f;
                explosionCurrentFrame = 0;
            }
        }
    }

    for (auto &missile : missiles) {
        if (missile.active && CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
        {enemyPosition_.x, enemyPosition_.y, 50, 50})) {
            missile.active = false;
            enemyHealth_ -= 10;

            if (enemyHealth_ <= 0) {
                enemyHealth_ = 0;
                // TODO: explode enemy sprite
            }
        }
    }
}

/**
 * @brief Display all resources & texts
 * 
 * @param rl 
 */
void Game::draw(rtype::RayLib &rl)
{
    if (isExploding) {
        rl.drawTextureRec(explosionSpriteSheet, explosionFrameRec, {playerPosition.x, playerPosition.y}, WHITE);
    } else {
        rl.drawTextureRec(playerSpriteSheet, playerFrameRec, {playerPosition.x, playerPosition.y}, WHITE);
        rl.drawRectangle(playerPosition.x, playerPosition.y - 10, 50, 5, GRAY);
        rl.drawRectangle(playerPosition.x, playerPosition.y - 10, static_cast<int>(50 * (playerHealth / 100.0f)), 5, GREEN);
    }

    for (const auto &missile : missiles)
        if (missile.active)
            rl.drawTextureRec(missileSpriteSheet, missileFrameRec, {missile.position.x, missile.position.y}, WHITE);

    for (const auto &asteroid : asteroids) {
        if (asteroid.active) {
            rl.drawRectangle(asteroid.position.x, asteroid.position.y, 50, 50, RED);
            rl.drawText(TextFormat("%d", asteroid.hitsRemaining), asteroid.position.x + 15, asteroid.position.y + 15, 20, WHITE);
        }
    }

    for (const auto &missile : enemyMissiles_)
        if (missile.active)
            rl.drawTextureRec(missileSpriteSheet, missileFrameRec, {missile.position.x, missile.position.y}, WHITE);

    rl.drawTextureRec(enemySpriteSheet, enemyFrameRec, {enemyPosition_.x, enemyPosition_.y}, WHITE);
    rl.drawRectangle(enemyPosition_.x, enemyPosition_.y - 10, 50, 5, GRAY);
    rl.drawRectangle(enemyPosition_.x, enemyPosition_.y - 10, static_cast<int>(50 * (enemyHealth_ / 300.0f)), 5, RED);

    rl.drawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    rl.drawText(TextFormat("Health: %d", playerHealth), 10, 40, 20, RED);
}


/**
 * @brief Display game over when player loses
 * 
 * @param rl 
 */
void Game::drawGameOver(rtype::RayLib &rl)
{
    rl.drawText("GAME OVER", 250, 250, 50, RED);
    rl.drawText(TextFormat("Score: %d", score), 250, 310, 20, WHITE);
    rl.drawText("Press ESC to exit", 50, 550, 20, LIGHTGRAY);
}
