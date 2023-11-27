#include<iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time() random seeding
#include <vector>

#include "Bullet.h"
#include "BulletPattern.cpp"
#include "Alien.cpp"
#include "EnemyWave.cpp"

//Basically the level selector in the game/ randomly loads up different enemy formations when all enemies of current wave are destroyed
//Calls on boss formation to be next when store appears

class WaveMgr
{
  public:
     WaveMgr(Alien **enemiesArr, const size_t maxE, int wave,  sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts, size_t *bulletIndex, float maxMusicVolume);
     void selectWave();
      bool  updateRemainingEnemies();
       void setBossSpawn(bool b, bool *bossMusicFader);
        void resetShopTime(float *timer, bool *shipAppear, bool *shopAppear, bool *shopOver);
         void manageMusicFades(bool (&fades)[6], float (&volumes)[3], sf:Music *mobBg, sf::Music *shopBg, sf::Music *bossBg );

};