#include<iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Bullet.h"

#ifndef _BulletPattern_H
#define _BulletPattern_H

//Different pattern shapes that bullets are spawned in each time a weapon is fired

class BulletPattern
{
  public:
     BulletPattern(sf::Sprite *id, sf::Texture *texture, Bullet **masterBulletArr, const size_t maxBull, sf::IntRect bulletImg, sf::VertexArray *verts, size_t *bulletIndex, int weapon = 0);
    BulletPattern();
    void addBullet(sf::VertexArray* v, Bullet *bul, size_t index);
     void setFireRadius(double r);
       void shoot(sf::Time deltaTime);
       void setBulletSpeed(double mod);
       void updateFireRate(sf::Time deltaTime);
       
       private:
         sf::Vector2f getFiringOrigin();
         bool checkBulletOwner();
         void spreadFire(sf::Time deltaTime);
         void singleFire(sf::Time deltaTime);
          void fanFire(sf::Time deltaTime);
          void randFire(sf::Time deltaTime);
        void setFiringRate(sf::Time rate);
        sf::Time getFiringRate();
};
#endif