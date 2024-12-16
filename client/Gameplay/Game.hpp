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
        bool inGameMenu;
        int playerHealth;
        int gameOver;
        float frameTime;
    
        void handleInput(rtype::RayLib &rl);
        void updateMissiles();
        void spawnObstacles();
        void checkCollisions();
        void saveScore();
        void handleInGameMenu(rtype::RayLib &rl, bool &inGameMenu);
        void draw(rtype::RayLib &rl);
        void drawGameOver(rtype::RayLib &rl);

        // enemies
        int enemyHealth;
        float enemyFireInterval;
        float enemyFireTimer;
        float enemySpeed;
        Vector2 enemyPosition;
        void updateEnemy();
        std::vector<Missile> enemyMissiles;
};

#endif //GAME_HPP