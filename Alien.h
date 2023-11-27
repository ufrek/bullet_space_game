#include<iostream>
#include <SFML/Graphics.hpp>
#include "GameObject.cpp"

#include "Bullet.h"
#include "BulletPattern.cpp"

// Loads all enemy types 
// There are multiple textures for each type of enemy as well as equipped bullet patterns and bullet types

class Alien : public GameObject
{
   public:
      Alien(sf::Vector2f position, sf::Vector2f scale, int textureIndex,sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts, size_t *bulletIndex, int weapon, int hp = 1, int pointValue = 100);
       Alien(); 
      void Move();
      void Move(float deltaTime);
      void setDirection(bool dir);
      void setSpeed(float s);
      sf::Sprite &getSprite();
      void setBoundaries(int xMin, int xMax, int yMin, int yMax);
      sf::Vector2f getSize();
      bool hit(int damage);
      int getValue();
      
      
       void shoot(sf::Time deltaTime);
       void updateFireRate(sf::Time deltaTime);
       void changeCurrentWeapon(int weapon);
      
      bool checkIfShooting();
       void setShootThreshold(int threshold);
       
       //movementPatterns
       void leftToRight(float deltaTime);
       void circle(float deltaTime);
       void figureEight(float deltaTime);
       void setRadius(double r);
        void setFiringRadius(double r);
      
      
};
