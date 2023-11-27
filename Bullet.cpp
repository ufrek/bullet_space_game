#include<iostream>
#include <SFML/Graphics.hpp>

#include "Bullet.h"

//UNUSED Bullet behavior. Moves straight down in a line

//RObert - just basic movment of projectiles in a line
//NOTE: Still need to fix textures not loading properly
//NOTE: still need to make disappear after being hit
class Bullet
{
  public:
     Bullet(sf::Texture *texture, sf::Vector2f vel, sf::Vector2f pos,  sf::IntRect rect)  
     {
         this->velocity = vel;
         this->bulletColor = rect;
          bulletS->setTexture(*texture);
		      bulletS->setTextureRect(rect);
		      bulletS->setPosition(pos);
        
     }
     
    Bullet() //empty constructor for container initialization
    {
    
    } 
     
     //move bullet down in a straight line at a specific speed
     void Move(float deltaTime)
     {
        
        mov.x = velocity.x * deltaTime;
  
         mov.y = velocity.y * deltaTime;  //move veritally down
  
         
         
        bulletS->move(mov);
        
        //implement collision check here next
     }
  
     //returns position of bullet sprite
     sf::Vector2f getPosition()
     {
         return bulletS->getPosition();
     }
     
     //returns sprite for drawing on window
     sf::Sprite *getSprite()
     {
       return bulletS;
     }
     
     //returns true if belongs to player for collision detection
     bool getBulletOwner()
     {
         if(bulletColor == sf::IntRect(0, 64, 16, 16))
           return true;
         else
           return false;
     }
   
     
   private:
     sf::Sprite *bulletS;
	    sf::Vector2f velocity;
     sf::Vector2f mov;  //amount to move bullet
     sf::IntRect bulletColor;
     bool isPlayersBullet;  //true for players, false for enemy  bullets
     
};
