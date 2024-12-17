#ifndef GAME_HPP
    #define GAME_HPP

#include "../raylib/raylib.hh"
#include <vector>

struct Missile {
    Vector2 position;
    float speed;
    bool active;
};

struct Asteroid {
    Vector2 position;
    int hitsRemaining;
    bool active;
};

class Game {
public:
    Game();
    void run(rtype::RayLib &rl);

private:
    void handleInput(rtype::RayLib &rl);
    void updateAnimations();
    void updateExplosion();
    void updateMissiles();
    void updateEnemy();
    void spawnAsteroids();
    void checkCollisions();
    void draw(rtype::RayLib &rl);
    void drawGameOver(rtype::RayLib &rl);

    // Player variables
    Vector2 playerPosition;
    float playerSpeed;
    int playerHealth;
    Texture2D playerSpriteSheet;
    Rectangle playerFrameRec;
    int currentFrame;
    float animationTimer;
    float animationSpeed;

    // Missile variables
    Texture2D missileSpriteSheet;
    Rectangle missileFrameRec;
    float missileAnimationTimer;
    float missileSpeed;
    std::vector<Missile> missiles;

    // Explosion variables
    Texture2D explosionSpriteSheet;
    Rectangle explosionFrameRec;
    bool isExploding;
    float explosionTimer;
    float explosionSpeed;
    int explosionCurrentFrame;

    // Enemy variables
    Vector2 enemyPosition_;
    float enemySpeed_;
    int enemyHealth_;
    Texture2D enemySpriteSheet;
    Rectangle enemyFrameRec;
    float enemyFireTimer_;
    float enemyFireInterval_;
    float enemyAnimationTimer;
    std::vector<Missile> enemyMissiles_;

    // Asteroid variables
    std::vector<Asteroid> asteroids;
    float asteroidSpawnTimer;
    float asteroidSpawnInterval;

    // Game state
    bool gameOver;
    int score;

    // Frame timing
    float frameTime;
};

#endif //GAME_HPP