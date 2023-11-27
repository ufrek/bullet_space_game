#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>


#ifndef _Bullet_H
#define _Bullet_H

//Robert:: Bullet behavior -moves based on a velocity, each bullet is owned by enemies or by player for collision detection
//Modified from: https://glusoft.com/tutorials/sfml/creating-bullet-hell-game

class Bullet
{

 
  public:
      //Decides bullet color, position, and velocity
      //Param:
          //texture : pointer to bullet sprite sheet
          // vel - velocity of bullet so that it can be moved each frame
          // pos - spawn position of bullet
          // rect - bullet color offset in sprite sheet
     Bullet(sf::Texture *texture, sf::Vector2f vel, sf::Vector2f pos,  sf::IntRect rect)  
     {
         this->velocity = vel;
         this->bulletColor = rect;
          bulletS.setTexture(*texture);
		      bulletS.setTextureRect(rect);
		      bulletS.setPosition(pos);
        
     }
     
    Bullet() //empty constructor for array initialization
    {
    
    } 
     
     //move bullet based on calculated velocity
     //Param
         //delta time - time since last frame 
     void Move(double deltaTime)
     {
        
        mov.x = velocity.x * deltaTime;
        mov.y = velocity.y * deltaTime;  //move veritally down
       
        bulletS.move(mov);
        
        //implement collision check here next
     }
  
     //returns position of bullet sprite
     sf::Vector2f getPosition()
     {
         return bulletS.getPosition();
     }
     
     //returns sprite for drawing on window
     sf::Sprite *getSprite()
     {
       return &bulletS;
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
     sf::Sprite bulletS;
	    sf::Vector2f velocity;
     sf::Vector2f mov;  //amount to move bullet
     sf::IntRect bulletColor;
     bool isPlayersBullet;  //true for players, false for enemy  bullets
 
};
#endif
