#include<iostream>
#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "BulletPattern.cpp"
#include "Alien.cpp"

//Spawns a Formation of enemies on screen - basically a "level" -spawns a new one when all enemies are defeated in current wave

class EnemyWave
{
     public:
    EnemyWave(Alien **enemiesArr, const size_t maxE, int wave,  sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts, size_t *bulletIndex);
    void addEnemy(Alien *a);
     void BuildWave();
     
     
     
     void UpdateRemainingEnemies();
     
     private:
       void BasicFormation();
        void circleFormation();
         void circleStrategy();
          void yourScrewed();
          void bossLevel();
};