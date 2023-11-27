#include<iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time() random seeding
#include "GameObject.cpp"
#include<vector>    //use this as a container to store bullets


#include "Bullet.h"
#include "BulletPattern.cpp"

#ifndef _Alien_
#define _Alien_

// Loads all enemy types 
// There are multiple textures for each type of enemy as well as equipped bullet patterns and bullet types
 

//Robert - refactored basic movements into classes and added bounds checking

//Alllows for 2 enemy textures
class Alien : public GameObject
{
      public:
      //Robert:: set position and scaling of aien as well as decide which of 2 textures for enemy,, also passes all pointers to main bulet variables to manage giant list of bullets
      //Param:
        //position - spawn point on the screen
        // scale - scales size of enemy on screen
        //texture index - which enemy type is displayed on screen - also decies which bullet pattern is equipped(Andrew)
        //bulletT - pointer to master spreadsheet for all bullets
        // masterBullets - pointer to master container for all bullets
        // maxB - maximum displayed bullets on screen
        // verts - vertex array for rendering all bullets on screen
        // bulletIndex - pointer to current bullet slot in master container
        // weapon - which weapon type is equipped to alien ship
        // hp - how many hits does it take to destroy the enemy
        // pointValue - how many points do you get for destroying the alien
        // movement - the movement behavior for this enemy
      Alien(sf::Vector2f position, sf::Vector2f scale, int textureIndex,  sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts,  size_t *bulletIndex, int weapon, int hp = 1, int pointValue = 100, int movement = 0) 
      : GameObject(position.x, position.y) //not used, but no time to clean up right now
      {
         Transform::setScale(scale.x, scale.y); //not used but not enough time to clean up
         Transform::setBounds(0,1920,0,1080);  //not used, but would be used to set bounds of movement for alien
          this->health = hp;
          this->value = pointValue;
          this->movementPattern = movement;
         // set random seed
         srand(time(NULL));
          
         //Andrew: Alien type configurations setup 
         switch (textureIndex){
             case 0:
                alienTexture.loadFromFile("enemy1.png");
                bulletColor =   sf::IntRect(128, 0, 64, 64);
                break;
             case 1:
                alienTexture.loadFromFile("enemy2.png");
                bulletColor =   sf::IntRect(0, 80, 8, 8);
                break;
             case 2:
                alienTexture.loadFromFile("bossBody.png");
                //std::cout<<"Check2";
                bulletColor =   sf::IntRect(64, 0, 64, 64);
                break;
             case 3:
                alienTexture.loadFromFile("bottomLeft.png");
                //std::cout<<"Check3";
                bulletColor =   sf::IntRect(128, 0, 64, 64);
                break;
             case 4:
                alienTexture.loadFromFile("bottomRight.png");
                //std::cout<<"Check4";
                bulletColor =   sf::IntRect(128, 0, 64, 64);
                break;
             case 5:
                alienTexture.loadFromFile("topLeft.png");
                //std::cout<<"Check5";
                bulletColor =   sf::IntRect(0, 80, 8, 8);
                break;
             case 6:
                alienTexture.loadFromFile("topRight.png");
                //std::cout<<"Check6";
                bulletColor =   sf::IntRect(0, 80, 8, 8);
                break;
             default:
                std::cout<<"Improper AlienTexture";
                break;
         
         }
         
         //Robert - sprite setup
         alienS.setTexture(alienTexture);
         alienS.setScale(scale.x, scale.y);
         alienS.setPosition(position.x, position.y);
        
        
          
         //Robert - equipped bullet pattern initialization
         this -> currentWeapon = weapon;
         shotPattern =  BulletPattern(&alienS, bulletT, masterBullets, maxB,  bulletColor, verts, bulletIndex, weapon);   
  
        
      }
      
      Alien() : GameObject(0,0)  //Robert:: empty constructor for container initialization in main
      {
    
      } 
      
      //unused: Needs to be cleaned up from GameObject class
      void Move()
      {
      }
      
      //Robert:: uses velocity and delta time to move sprite across screen
      // Param:: 
        //deltaTime - time since previous frame used to calculate distance travelled
      void Move(float deltaTime)
      {
        switch(movementPattern)
        {
             case 0:
              leftToRight(deltaTime);
              break;
            case 1:
              figureEight(deltaTime);
              break;
            case 2:
              circle(deltaTime);
              break;
            case 3:
              break;
            default:
              std::cout << "Invalid movement Pattern" << std::endl;
        }
         
      }
      
 //MOVEMENT PATTERNS  
       //Robert:: Basic movement from left to right sides of screen  
         // Param:: 
        //deltaTime - time since previous frame used to calculate distance travelled 
      void leftToRight(float deltaTime)
      {
         velocity.x = 0.f;
          velocity.y = 0.f;
          //std::cout<< "Moving" << std::endl;
          if(isMovingRight)
          {
            velocity.x = speed * deltaTime;
          }
          else
          {
            velocity.x = -speed * deltaTime;
          }
          
           
          //boundaries check
          sf::Vector2f size = getSize();
          int* bounds = Transform::getBounds(boundaries);
          sf::Vector2f pos = alienS.getPosition(); 
             if (pos.x > 1920 - size.x)   //right side bounds check
        {
             if(velocity.x > 0)
               velocity.x = 0.f;
            pos.x = bounds[1] - size.x;
            alienS.setPosition(pos.x, alienS.getPosition().y);
        }
        else if(pos.x < 0)     //left side bounds check
        {
          if(velocity.x < 0)
           velocity.x = 0.f;
            //pos.x = 0;
            alienS.setPosition(0, alienS.getPosition().y);
        }
        
        
        //not used
        if(pos.y >  1080 - size.y)    //bottom bounds check hardcoded because bounds didn't work for some reason....
        {
         
          if(velocity.y > 0.f)
            velocity.y = 0.f;
 
       
           
        }
        if( pos.y < 0)   //top bounds check
        {  
      
           if(velocity.y < 0.f)
             velocity.y = 0.f;
             

        }
          
         // std::cout << "Moving " << velocity.x << std::endl;
          alienS.move(velocity);
          
          //update position of sprite and movement direction
          
          Transform::setPosition(pos.x, pos.y);
    
          if(pos.x >= bounds[1] - size.x)
          {
              isMovingRight = false;
          }
          if(pos.x <= bounds[0])
          {
            isMovingRight = true;
          }
      }
      
      
       //Andrew:: Alien moves in a circle
         // Param:: 
        //deltaTime - time since previous frame used to calculate distance travelled
      void circle(float deltaTime)
      {
        sf::Vector2f position = {(float)((radius*cos(moveT*(Pi/2)))),(float)((radius*sin(moveT*(Pi/2))))};
//        std::cout<<"Cos1: "<<cos(moveT*Pi)<<std::endl;
//        std::cout<<"Sin1: "<<sin(moveT*Pi)<<std::endl;
//        std::cout<<"moveT1: "<<moveT<<std::endl;
        float xvel = xneg*position.x;
        float yvel = yneg*position.y;
        sf::Vector2f vel = {xvel,yvel};
        alienS.move(vel);
        moveT = moveT + deltaTime;
        if(moveT==1){
          moveT=-1.0f;
        }
      }
      
      //Andrew: Figure 8 movement of ships
      //NOTE: You need to set radius if you want something different from default circle size
       void figureEight(float deltaTime)
       {
      
      
        sf::Vector2f position = {(float)((radius*cos(moveT*(Pi/2)))),(float)((radius*sin(moveT*(Pi/2))))};
//        std::cout<<"Cos1: "<<cos(moveT*Pi)<<std::endl;
//        std::cout<<"Sin1: "<<sin(moveT*Pi)<<std::endl;
//        std::cout<<"moveT1: "<<moveT<<std::endl;
        float xvel = xneg*position.x;
        float yvel = yneg*position.y;
        sf::Vector2f vel = {xvel,yvel};
        alienS.move(vel);
        moveT = moveT + deltaTime;
        if(moveT>1.0f){
          switch (count){
              case 1:
                //std::cout<<"Check1";
                yneg=-1.0f;
                moveT=-1.0f;
                count=2;
                break;
              case 2:
                //std::cout<<"Check2";
                yneg=1.0f;
                xneg=-1.0f;
                moveT=-1.0f;
                count=3;
                break;
              case 3:
                //std::cout<<"Check3";
                yneg=-1.0f;
                moveT=-1.0f;
                count=4;
                break;
              case 4:
                //std::cout<<"Check4";
                yneg= 1.0f;
                xneg= 1.0f;
                moveT=-1.0f;
                count=1;
                break;
              default:
                std::cout<<"bad";
                count=1;
                moveT=0.99f;
                break;
                }
        }
      }
      
      
      
      
      
      //Robert:: set which direction alien will move, left or right in back and forth movement
      //Param:
      //       dir - true to move right, false to move left
      void setDirection(bool dir)
      {
        isMovingRight = dir;
      }
      
    //Robert:: returns Alien Sprite size after taking into account the scaling done on the sprite
    sf::Vector2f getSize()
    {
        sf::Vector2f size;
        size.x = alienS.getTexture()->getSize().x * alienS.getScale().x,
        size.y = alienS.getTexture()->getSize().y * alienS.getScale().y;
        return size;
    }
      
      //Robert:: set movement speed  
    void setSpeed(float s)
    {
      speed = s;
    }
    
    //Robert:: return sprite for drawing on main window
    sf::Sprite &getSprite()
    {
       return alienS;
    }
    
    //UNUSED: wanted to store screen height and width for use instead of hardcoding - Robert
     void setBoundaries(int xMin, int xMax, int yMin, int yMax)
    {
      Transform::setBounds(xMin, xMax, yMin, yMax);
    }
    
    //Robert :: checks randomly to see if alien should shoot
    bool checkIfShooting()  //subtracting delta time does not work. I don't know why, alternative was used here
    {
         shootChance =  (float)(rand() % 1000) ;   // check probability of random number generator to see if we should shoot
       if(shootChance <= shootThreshold)
         return true;
       else
        return false;
    }
    
     //Robert:: Set to 1000 to constantly fire, otherwise changes the threshold/1000 percent chance of firing
    void setShootThreshold(int threshold)
    {
      shootThreshold = threshold;
    }
    
    //BULLET PATTERN SHOOT METHODS
    
    //Robert:: delegates shooting to equipped bullet pattern
    //Param
      // deltaTime - time passed since last fram,e - used for checking fire rate and bullet spawn pattern
     void shoot(sf::Time deltaTime)
    {
      shotPattern.shoot(deltaTime);
    }
    
    //Robert:: checks if weapon is ready to be fired again - run every frame to cehck fire rate
    void updateFireRate(sf::Time deltaTime)
    {
      shotPattern.updateFireRate(deltaTime);
    }
    
    //Robert:: changes equipped weapon to new weapon - used in ship for upgrades
    //Param:
      //weapon - ID of weapon to be equipped
    void changeCurrentWeapon(int weapon)
    {
      currentWeapon = weapon;
      shotPattern.setCurrentWeapon(weapon);
    }
    
    //Robert:: called when enemyis hit, subtracts player weapon power from health
    //Param:
      //damage : how much power the player shit currently has
    bool hit(int damage)
    {
        health -= damage;
        if(health <= 0)
            return true;
        
        else
          return false;
    }
    
    //Robert:: returns score value of enemy ship when destroyed to update the score
    int getValue()
    {
      return value;
    }
   
   ///Robert:: Sets radius for enemy movements
    void setRadius(double r)
    {
      radius = r;
    }
   
   //Robert:: sets blast radius for equipped enemy guns
     void setFiringRadius(double r)
     {
       shotPattern.setFireRadius(r);
     }
    
      protected:
       //has pos x and y as well as scale
       sf::Texture alienTexture;
       sf::Sprite alienS;
       bool isMovingRight = true;   //set for basic back and forth movement
       float speed = 5.f;  //speed of movement
       const int XMAX = 1080; //furthers right point on screen
       const int XMIN = 1;  //furthest left point on screen
       int boundaries[4];  //UNUSEd - set values for each end of the screen
       sf::Vector2f velocity;  //direction and speed of enemy
     
       BulletPattern shotPattern;  //equipped weapon for enemy
       sf::IntRect bulletColor;   //color of enemy's bullets - blue, green, or purple for enemies
       int currentWeapon;  //currenlty equipped weapon
 
       int value = 100;  //point value for when enemy is destroyed
       int health = 1;  //enemy hit points, destroyed when hit 0
       
     int movementPattern = 0; //which movement pattern the enemy will fly in
     
     //Andrew:
     //semiCircle variables
     double Pi= 3.14159265358979323846; 
     int count=1;
     float moveT=0.0f;
     float xneg=1.0f;
     float yneg=1.0f;
     double radius = 1;
     
     //Robert :: bullet variables 
     float shootChance; //randomly generated number
     float shootThreshold = 10; //if Random number is less than this number, shoot weapon
  
};
#endif
