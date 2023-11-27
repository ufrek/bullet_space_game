#include<iostream>
#include <SFML/Graphics.hpp>
#include "GameObject.cpp"
 
#include "Bullet.h"
#include "BulletPattern.cpp"

//Manages Ship's position, size, equipped weapon, and current state if it gets hurt to introduce frames of invincibility

class Ship : GameObject
{
   public:
       Ship(sf::Vector2f position, sf::Vector2f scale, sf::Texture &bulletT, Bullet **masterBullets, const size_t maxBullets, sf::VertexArray *verts, int weapon, size_t *bulletIndex);
      void Move(sf::Vector2f *direction, float deltaTime);
      void Move();
      void setSpeed(float s);
      
      
      float getSpeed();
     void setFiringRate(sf::Time rate);
      sf::Time getFiringRate();
      
      sf::Vector2f getSize();
      void setBoundaries(int xMin, int xMax, int yMin, int yMax);
      int getHealth();
      void setHealth(int h);
      void manageShipStatus(float deltaTime);
      void hit();
      void updateFireRate(sf::Time deltaTime);
};
