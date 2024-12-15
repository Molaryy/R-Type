#ifndef GAME_HPP
    #define GAME_HPP

#include "../raylib/raylib.hh"
#include <vector>

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
    public:
        Game();
        void run(rtype::RayLib &rl);

    private:
        Vector2 playerPosition;
        float playerSpeed;
        float missileSpeed;
        std::vector<Missile> missiles;
        std::vector<Obstacle> obstacles;
        float obstacleSpawnTimer;
        float obstacleSpawnInterval;
        int score;
    
        void handleInput(rtype::RayLib &rl, float frameTime);
        void updateMissiles(float frameTime);
        void spawnObstacles(float frameTime);
        void checkCollisions();
        void draw(rtype::RayLib &rl);
};

#endif //GAME_HPP