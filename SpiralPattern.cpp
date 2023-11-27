#include<iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Bullet.cpp"
#include "BulletPattern.cpp"


/// Robert :: NOT USED.....Can't figure out proper inheritance structure
#ifndef _SpiralPattern_
#define _SpiralPattern_
class SpiralPattern : BulletPattern
{
  public:
    SpiralPattern(sf::Sprite *id, sf::Texture* texture,  Bullet **masterBulletArr, const size_t maxBull,  sf::IntRect bulletImg, sf::VertexArray &verts) :
        BulletPattern(id,  texture, masterBulletArr, maxBull, bulletImg, verts)
        
    {
     
    }
    
    void shoot(float deltaTime)
    {
        float dt = deltaTime * 1000; //convert to milliseconds
        
        sf::Vector2f position = owner->getPosition();  //update origin of fire
        
            // Add new red bullet if timer hits bulletTime threshold
              
            if (counterTime > firingRate)  //&& counterWave < 1)  // no idea what counterWave really does
             {
            	  if (numBullet == maxBullets) {
            		  numBullet = 0;
            		//counterWave++;
            	}
            
            	if (bullets[numBullet] != NULL) {
            		delete(bullets[numBullet]);
            		bullets[numBullet] = NULL;
            	}
            
            	float dirX = radius*cos((size_t) (numBullet%180) / Pi);
            	float dirY = radius*sin((size_t) (numBullet%180) / Pi);
             
             //check owner and select correct bullet
             sf::IntRect bColor;
            
            bColor =  sf::IntRect(0, 64, 16, 16);  //red bullet for player
            bColor = sf::IntRect(128, 0, 64, 64); //blue bullet for enemies
             
             
            	Bullet *bul = new Bullet(bulletTexture, sf::Vector2f(dirX * speedModifier, dirY * speedModifier), position, bulletColor); //offset in the sprite sheet for red bullets
            	bullets[numBullet] = bul;
            	addBullet(&vertices, bul, numBullet);
            	numBullet++;
             
            	counterTime = counterTime - firingRate;  //count back bullet time when 
            }
        
        counterTime += dt;
    }
    
  
     SpiralPattern& operator=(SpiralPattern const& other) override
    {
        if (auto* aOther = dynamic_cast<SpiralPattern const*>(&other))
        {
            *this = *aOther;
        }
        return *this;
    }
    
 
      
      
      
};
#endif