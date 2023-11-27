#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "GameObject.cpp"

#include "Bullet.h"
#include "BulletPattern.cpp"



#ifndef _Ship_
#define _Ship_

//Manages Ship's position, size, equipped weapon, and current state if it gets hurt to introduce frames of invincibility



class Ship : GameObject
{
    public:
    //Robert::decided position, scale, and equipped weapon for the ship
      //Param:
        //position -- starting positon of player ship
        // scale - how large we want to scale the ship
        // bulletT - main sprite sheet for all bullets
        //masterBullets - main container for all bullets
        //maxBullets - maximum amount of bullets onscreena at once
        //verts - main vertex array for rendering bullets
        //weapon - which bullet pattern to equip for firing
        //bulletIndex - current slot to create new bulet in main bullet container
      Ship(sf::Vector2f position, sf::Vector2f scale, sf::Texture *bulletT, Bullet **masterBullets, const size_t maxBullets, sf::VertexArray *verts, int weapon, size_t *bulletIndex) : GameObject(position.x, position.y)
      {
        Transform::setScale(scale.x, scale.y);
        Transform::setBounds(0,1920,0,1080);
        //this->vertices = verts;
        
        
         //load in texture and setScale
        shipTexture.loadFromFile("ship.png");
        shipS.setPosition(position.x, position.y);
        shipS.setTexture(shipTexture);
        shipS.setScale(scale.x, scale.y);
        
        health = 3;
        
    
         bulletColor =  sf::IntRect(0, 64, 16, 16); //set red bullets 
         this -> currentWeapon = weapon;
         shotPattern =  BulletPattern(&shipS, bulletT, masterBullets, maxBullets,  bulletColor, verts, bulletIndex, weapon);   

        setBaseFiringRate();
      }
 
 /////////   BALANCE THIS BETEER IF HAVE TIME --BASE FIRING STATS   
     //Robert:: set basic wrapon stats depending on which weapon is initially selected at application start  
      void setBaseFiringRate()
      {
          switch(currentWeapon)
          {
              case 0:
                setFiringRate(sf::seconds(.6));
                break;
              case 1:
               setFiringRate(sf::seconds(.5));
                break;
              default:
                std::cout << "Base Firing Rate not set. Please add case in Ship.cpp setBaseFiringRate()" << std::endl;
          }
      }
      
      
      //Robert::moves ship on screen based on detected input
      //Param:
        //direction - which way the ship is moving
        // deltaTime - time since previous frame
      void Move(sf::Vector2f *direction, float deltaTime)    //include Time.deltatime at a later time
      {
          
          velocity.x = direction->x * speed * deltaTime;
          velocity.y = direction->y * speed * deltaTime;
          
  
          
         //bounds check for motion
          sf:: Vector2f pos = shipS.getPosition();
          int* bounds = Transform::getBounds(boundaries);  //broken for some reason, don't use yet, values are hardcoded fornow
          sf::Vector2f size = getSize();
    
           if (pos.x > bounds[1] - size.x)   //right side bounds check
        {
             if(velocity.x > 0)
               velocity.x = 0.f;
        }
        else if(pos.x < 0)     //left side bounds check
        {
          if(velocity.x < 0)
           velocity.x = 0.f;
            shipS.setPosition(0, shipS.getPosition().y);
        }
        
        if(pos.y >  1080 - size.y)    //bottom bounds check hardcoded because bounds didn't work for some reason....
        {
         
          if(velocity.y > 0.f)
            velocity.y = 0.f;
          pos.y = 1080 - size.y;
        }
        
        if( pos.y < 0)   //top bounds check
        {  
      
           if(velocity.y < 0.f)
             velocity.y = 0.f;
             
            pos.y = 0;

        }
        
        shipS.move(velocity);  //update position of sprite
     
      }
      
      //UNUSED: general game object behavior for move
      void Move()
      {
      }
    
    //Robert::returns size of sprite of ship based on scaling of the sprite
    sf::Vector2f getSize()
    {
        sf::Vector2f size;
        size.x = shipS.getTexture()->getSize().x * shipS.getScale().x,
        size.y = shipS.getTexture()->getSize().y * shipS.getScale().y;
        return size;
    }
    
    
    
    //Andrew::sets movement speed of ship
    //Param
        //s - how fast we want the player to move
    void setSpeed(float s)
    {
      speed = s;
    }
    
    //Andrew:: returns movement speed of player
     float getSpeed()
    {
      return this->speed;
    }
    //Andrew-shopUpdate
    
    //Andrew:: modifies how fast lpayer can fire equipped weapon
    //Param
        //rate - how fast the weapon can be fired
    void setFiringRate(sf::Time rate)
    {//Andrew-shopUpdate
        this->shotPattern.setFiringRate(rate);
    }
    
    
    //Andrew:: gets how fast player can fire weapon
    sf::Time getFiringRate()
    {
    //Andrew-shopUpdate
      return this->shotPattern.getFiringRate();  
    }
    
    //Robert::returns ship sprite for drawing on window
    sf::Sprite &getSprite()
    {
      return shipS;
    }
    
    //UNUSED: supposed to store values for screen bounds
    void setBoundaries(int xMin, int xMax, int yMin, int yMax)
    {
      Transform::setBounds(xMin, xMax, yMin, yMax);
    }
    
    //Robert::called when ship is hit. Opens a 3 second window of invulnerability between ship hits
    //Param
      //hitSound - pointer to sound variable that should play when player is hurt
    void hit(sf::Sound *hitSound)
    {
      if(isVulnerable)
      {
        //std::cout << "Health DOwn"<<  health << std::endl;
        health--;  
        std::cout << health << std::endl;
        isVulnerable = false;  
        timeUntilVulnerable = coolDownTime;
        if(health > 0)
        {
            hitSound->play();
        }
      }
        
    }
    
    //Robert::returns health value for use in UI 
    int getHealth()
    {
      return this->health;
    }
    
    //Andrew:: used to add health when bought in shop
    //Param
      //h - how much health the player should have after powering up
    void setHealth(int h)
    {
      this->health = h;
    }
    
    //Robert:: shoots equipped weapon
    //Param
      //dektaTime - how much time has elapsed since previous frame
    void shoot(sf::Time deltaTime)
    {
      shotPattern.shoot(deltaTime);
    }
    
    
    //Robert:: checks to see when gun can be fired based on fire rate
      //Param
          //detaTime - time passed since previous frame used to count time elapsed since last fire action
    void updateFireRate(sf::Time deltaTime)
    {
      shotPattern.updateFireRate(deltaTime);
    }
    
   
   //Robert::uses Invincible cool down time to allow player to recover themselves if they get hit
   //TODO: make character blink or something when they get hit
   //Param::
     //deltaTime - how muich time eleapsed since previous frame - calculated how long player is invincible after getting hit 
    void manageShipStatus(float deltaTime)
    {
        if(isVulnerable == false)
        {
            timeUntilVulnerable = timeUntilVulnerable - deltaTime;
            if(timeUntilVulnerable <= 0)
            {
              isVulnerable = true;
            }
        }
    }
    
    //Andrew::swaps out equipped weapon for a different firing pattern
    //Param
      //weapon - ID of weapon we want to equip
    void changeCurrentWeapon(int weapon)
    {
      currentWeapon = weapon;
      shotPattern.setCurrentWeapon(weapon);
    }
    
    
    //weapon Damage Functions
      //Robert:: used in calculating damage against enemies when they get hit by player bullets
    int getWeaponPower()
    {
      return weaponPower;
    }
    
    //Andrew:: used for upgrading weapon power in shop
    void setWeaponPower(int pow)
    {
      this->weaponPower = pow;
    }
    
    protected:
       //has pos x and y as well as scale
       sf::Texture shipTexture;
       sf::Sprite shipS;
       float speed = 150.f;
       sf::Vector2f velocity;
       sf::VertexArray vertices;
       
       sf::Texture *bulletTexture;
      
       int boundaries[4];  //check to see this returns correct values later
       
       int health = 3;
       bool isVulnerable = true;   //allows for invincibility frames after user gets hit
       float timeUntilVulnerable = 3.f;  //3 second cool down before you can get hit again
       float coolDownTime = 3.f; //how long until user can get hit again after being set to invincible
       
       
       //bullet pattern
       BulletPattern shotPattern;
       sf::IntRect bulletColor; //offset in sprite sheet for specific bullet sprite
       
       int currentWeapon = 0;
       int weaponPower = 1;  //default firePower of 1
};
#endif
