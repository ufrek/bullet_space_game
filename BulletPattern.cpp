#include<iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Bullet.h"


#ifndef _BulletPattern_
#define _BulletPattern_

//Different pattern shapes that bullets are spawned in each time a weapon is fired

class BulletPattern 
{
  public:
    //Robert:: Initializes weapon type
    //Param
      //id - owner of the bullet- used to track position to fire from
      //texture - pointer to bullet sprite sheet
      // masterBulletArr - master container for bullets
      //maxBull - maximum number of bullets displayable
      //bulletImg - which bullet type will be displayed when shooting
      //verts - master vertex array for rendering bullets
      //bulletIndex - current slot in container to use when creating a bullet - used for ALL bullets in game
      //weapon - specific type of fire pattern to use
    BulletPattern(sf::Sprite *id, sf::Texture *texture, Bullet **masterBulletArr,  const size_t maxBull, sf::IntRect bulletImg, sf::VertexArray *verts, size_t *bulletIndex, int weapon = 0)
    {
        this->owner = id;
        this->bulletTexture = texture;
        this->bullets = masterBulletArr;
        this->maxBullets =  maxBull;
        this ->bulletColor = bulletImg;
        this->vertices = verts;
        this->currentWeapon = weapon;
        this->numBullet = bulletIndex;
         
        
        //initialize patterns
        counterTime = 0;
        counterWave = 0;
        firingRate = sf::seconds(.1);  //40 and 100 also good values
        radius = 20;
    }
    
    BulletPattern() //empty constructor for declaration of bullet pattern
    {
    }
    
    
    //Robert Adds new bullet to master bullet container and master vertex array for rendering
       //Source: https://glusoft.com/tutorials/sfml/creating-bullet-hell-game
      void addBullet(sf::VertexArray* v, Bullet *bul, size_t index)
      {

          // Get the quad contained in the vertex array
          sf::Vertex* quad = &(*v)[index*4];
         
          sf::Sprite *spr = bul->getSprite();
         
          // Set the position of the sprite
          quad[0].position = sf::Vector2f(spr->getPosition().x, spr->getPosition().y);
          
          quad[1].position = sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width, 
                                          spr->getPosition().y);
          
          quad[2].position = sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width, 
                                          spr->getPosition().y + spr->getTextureRect().height);
          
          quad[3].position = sf::Vector2f(spr->getPosition().x, spr->getPosition().y +
                                          spr->getTextureRect().height);
           
          // Set the texture of the sprite
          quad[0].texCoords = sf::Vector2f(spr->getTextureRect().left, spr->getTextureRect().top);
          
          quad[1].texCoords = sf::Vector2f(spr->getTextureRect().left + spr->getTextureRect().width,
                                            spr->getTextureRect().top);
          
          quad[2].texCoords = sf::Vector2f(spr->getTextureRect().left + spr->getTextureRect().width,
                                            spr->getTextureRect().top + spr->getTextureRect().height);
          
          quad[3].texCoords = sf::Vector2f(spr->getTextureRect().left, spr->getTextureRect().top +
                                            spr->getTextureRect().height);
    }
    
     //Robert: set radius for circle of fire around ship
     void setFireRadius(double r)
     {
       this->radius = r;
     }
     
     //Robert - set how fast weapon can be fired
     void setFiringRate(sf::Time rate)
     {
       this->firingRate = rate;
     }
     
     //Robert:: returns how fast weaon can be fired
     sf::Time getFiringRate()
     {
       return this->firingRate;
     }
     
     //Robert::Sets how fast bullets will move from origin of fire
    void setBulletSpeed(double mod)
    {
        this->speedModifier = mod;
    }
    
    //Robert:: set which firing pattern to use in this instance
    void setCurrentWeapon(int weapon)
    {
      this->currentWeapon = weapon;
    }

     
     //Robert::checks time for firing rate then fires off equipped bullet pattern
     //Param
       //deltaTime - time since last frame - used to see if enough time has elapsed to shoot again
     void shoot(sf::Time deltaTime)
    {
        
            // Add new bullet if timer hits bulletTime threshold
              
           if(fireTimer > fire)
           {
               fireTimer = fireTimer - deltaTime;
           }
           else
           {
    			    // Reset the number of bullet when the max is reached
    			    if (*numBullet == maxBullets) 
              {
    				    *numBullet = 0;
			        }

        			// Delete the previous bullet
        			if (bullets[*numBullet] != NULL) 
              {
        				delete(bullets[*numBullet]);
        				bullets[*numBullet] = NULL;
        			}
              
  
  //ADD BULLET PATTERNS HERE            
              //now that cleanup is done, shoot weapon
              switch(this->currentWeapon)
                     {
                         case 0:
                              singleFire(deltaTime);
                              break;
                          case 1:
                              spreadFire(deltaTime);
                              break;
                          case 2:
                              fanFire(deltaTime);//Andrew Enemy/Bullet patterns
                              break;
                          case 3:
                              randFire(deltaTime); //Andrew
                              break;
                         default:
                           std::cout << "Invalid Fire Pattern" << std::endl;
                     }
              
              fireTimer = firingRate; 
           }  
      
    }
    
   
    

    //Robert::Checked every frame to see if weapon is available to fire
    //Param
      //deltaTime - time elapsed since last frame
    void updateFireRate(sf::Time deltaTime)
    {
      fireTimer = fireTimer - deltaTime; 
    }
   
   
   private:
   
   //Robert:: Checks if bullet is player's or enemy's - determines if bullet moves upwards or downwards
   bool checkBulletOwner()
   {
       if(bulletColor == sf::IntRect(0, 64, 16, 16))
       {
           return true;
       }
       else
         return false;
   }
   
   //Robert:: returns position on enemy or alien sprite to fire from at any point in time (i.e. the top middle of the sprite or bottom of the sprite)
   sf::Vector2f getFiringOrigin()
   {
        sf::Vector2f position = owner->getPosition();
        position.x = position.x + ((owner->getTexture()->getSize().x * owner->getScale().x) / 2.9); //get x halfway across sprite
        
        // if player bullet we want Sprite top, if alien, we want bottom
       if(checkBulletOwner()) 
       {
         
          return position;
       }
       else //Alien shoots from bottom
       {
           //get halfway point on alien Sprite's bottom 
           position.y = position.y + (owner->getTexture()->getSize().y * owner->getScale().y);
          return position; 
       }
   }
   
///////// BULLET PATTERNS
  //Robert::Shoots 3 bullets at once in a triangular pattern
    //Param
      //deltaTime -unused, but potentially could be used to alter how much the pattern spreads
    void spreadFire(sf::Time deltaTime)
    {
          
            sf::Vector2f position = getFiringOrigin(); //update origin of fire
        
           for(int i=0;i<3;i++)
          {
                  // Calculate the coordinate of the velocity vector
          			float x = radius*cos((size_t) (*numBullet%3) +2/ Pi);
          			float y = radius*sin((size_t) (*numBullet%3) +2/ Pi);
                sf::Vector2f velocity;
                
                
               
                //if alien ship, we want it to shoot down
                if(!checkBulletOwner())
                {
                    //velocity = sf::Vector2f((x*0.03), abs((-y*0.01)-0.5)); //use this for sin moving enemies for a cool sideways shot
                    velocity = sf::Vector2f((x*0.03), .3);
                } 
                else
                {
                  velocity = sf::Vector2f((x*0.01), (-y*0.01)-0.5);
                }
          
          			Bullet *bul = new Bullet(bulletTexture, velocity, position, bulletColor);
          			bullets[*numBullet] = bul;
          			addBullet(vertices, bul, *numBullet);
          
          			// update number and counter
          			*numBullet = *numBullet + 1;
            }
             
    }  
    
    //Andrew: shoots single bullet from top of ship
      //Param : deltaTIme - unused but could affect certain behaviors
    void singleFire(sf::Time deltaTime)
    {
         int shots=1;
        sf::Vector2f position = getFiringOrigin(); //update origin of fire
       for(int i=0;i<shots;i++)
              {
                int calc = *numBullet%shots;
                      // Calculate the coordinate of the velocity vector
              			float x = radius*cos((size_t) (calc) / Pi);
              			float y = radius*sin((size_t) (calc)+1 / Pi);
                     sf::Vector2f velocity;
                     
                     //if alien ship, we want it to shoot down
                     if(!checkBulletOwner())
                    {
                         velocity = sf::Vector2f((0), (-y*0.01)+0.3);
                    } 
                    else
                    {
                      velocity = sf::Vector2f((0), (-y*0.01)-0.3);
                    }
                        
                    
                    
                    
              			Bullet *bul = new Bullet(bulletTexture,velocity, position, bulletColor);
              			bullets[*numBullet] = bul;
              			addBullet(vertices, bul, *numBullet);
                      
              			// update number and counter
              			*numBullet = *numBullet + 1;
                }
    } 
    
    //Andrew:: shoots out an arc of bullets from the ship
      //Param: deltaTime - unused but can alter shape of arc
     void fanFire(sf::Time deltaTime)//Andrew Enemy/Bullet patterns
    {
          
            sf::Vector2f position = getFiringOrigin(); //update origin of fire
        
           for(int i=0;i<10;i++)
          {
                  // Calculate the coordinate of the velocity vector
          			float x = radius*cos((size_t) (*numBullet%10) / Pi);
          			float y = radius*sin((size_t) (*numBullet%10) / Pi);
                sf::Vector2f velocity;
                
                
               
                //if alien ship, we want it to shoot down
                if(!checkBulletOwner())
                {
                    //velocity = sf::Vector2f((x*0.03), abs((-y*0.01)-0.5)); //use this for sin moving enemies for a cool sideways shot
                    velocity = sf::Vector2f((x*0.03), .3);
                } 
                else
                {
                  velocity = sf::Vector2f((x*0.01), (-y*0.01)-0.5);
                }
          
          			Bullet *bul = new Bullet(bulletTexture, velocity, position, bulletColor);
          			bullets[*numBullet] = bul;
          			addBullet(vertices, bul, *numBullet);
          
          			// update number and counter
          			*numBullet = *numBullet + 1;
            }
             
    }  //Andrew Enemy/Bullet patterns
    
    //Andrew:: randomly combines two patterns of arcs to create more chaotic pattern
    void randFire(sf::Time deltaTime)//Andrew Enemy/Bullet patterns
    {
          
            sf::Vector2f position = getFiringOrigin(); //update origin of fire
        
           for(int i=0;i<15;i++)
          {
                  // Calculate the coordinate of the velocity vector
                      float x = radius*cos((size_t) (rand()%180)/ (2*Pi));
                      float y = radius*sin((size_t) (rand()%180)/ (2*Pi));
                sf::Vector2f velocity;
                
                
               
                //if alien ship, we want it to shoot down
                if(!checkBulletOwner())
                {
                    //velocity = sf::Vector2f((x*0.03), abs((-y*0.01)-0.5)); //use this for sin moving enemies for a cool sideways shot
                    velocity = sf::Vector2f((x*0.03), (y*0.01)+0.3);
                } 
                else
                {
                  velocity = sf::Vector2f((x*0.01), (-y*0.01)-0.5);
                }
          
                      Bullet *bul = new Bullet(bulletTexture, velocity, position, bulletColor);
                      bullets[*numBullet] = bul;
                      addBullet(vertices, bul, *numBullet);
          
                      // update number and counter
                      *numBullet = *numBullet + 1;
            }
             
    }
   
    protected:
      double Pi  = 3.14159265358979323846;
      size_t maxBullets;
     
     
     sf::Sprite *owner = NULL;
     sf::Texture* bulletTexture = NULL;
     sf::Vector2f velocity = sf::Vector2f(0,0);
     sf::VertexArray *vertices;
     Bullet **bullets; //store master Bullet container for reference in firing
     bool isPlayerBullet = false;
     
     
     //pattern variables
     double counterTime = 0; //unused
     double counterWave = 0; //unused
     size_t *numBullet;
     sf::IntRect bulletColor;
     
     double radius = 20; // radius of the circle of fire
     double speedModifier = .01;
     
     int currentWeapon = 0;
     
     
     
     //Andrew's Fire logic
     sf::Time fireTimer; //counter to count up time until shooting
     sf::Time firingRate = sf::milliseconds(1000);  //how fast bullets are to shoot
     sf::Time fire = sf::milliseconds(0);
     
      

};
#endif