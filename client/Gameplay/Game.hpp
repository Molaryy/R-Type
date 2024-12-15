#ifndef GAME_HPP
    #define GAME_HPP

#include <vector>
#include "../raylib/raylib.hh"

struct Missile {
    Vector2 position;
    float speed;
    bool active;
};

struct Obstacle {
    Vector2 position;
    int hitsRemaining;
    bool active;
};

class Game {
private:
    Vector2 playerPosition;
    float playerSpeed;

    std::vector<Missile> missiles;
    float missileSpeed;

    std::vector<Obstacle> obstacles;
    float obstacleSpawnTimer;
    float obstacleSpawnInterval;

    int score;

    void handleInput(float frameTime);
    void updateMissiles(float frameTime);
    void spawnObstacles(float frameTime);
    void checkCollisions();
    void draw();

public:
    Game();
    void run();
};

#endif // GAME_HPP
