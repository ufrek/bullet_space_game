#include<iostream>
#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "BulletPattern.cpp"
#include "Alien.cpp"


#ifndef _EnemyWave_
#define _EnemyWave_

//Spawns a Formation of enemies on screen - basically a "level" -spawns a new one when all enemies are defeated in current wave

class EnemyWave
{
  
  public:
    //Robert:: Initializes Wave pases all master variables from main for management
    //Param:
      //enemiesArr - pointer to master container for all enemies
      //maxE - maximum number of enemies that can be displayed at once
      // wave - which formation to spawn
      //bulletT - sprite sheet for all bullets
      //materBullets - master container for all bullets on screen
      // maxB - maximum bullets to be displayed on screen
      // verts - master vertex array for rendering bullets
      //bulletIndex - current slot in master container to use for adding a new bullet
    EnemyWave(Alien **enemiesArr, const size_t maxE, int wave,  sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts, size_t *bulletIndex)
    {
        this->enemies = enemiesArr;
        this->maxEnemies = maxE;
        
        this->bulletTexture = bulletT;
        this->bullets = masterBullets;
        this->maxBullets = maxB;
        this->vertices = verts;
        this->currentBulletIndex = bulletIndex;
        this->waveID = wave;
        
        //BuildWave();  //test if waves are spawning
    }
    
    
  //Robert:: adds a new enemy to the formation of enemies
    void addEnemy(Alien *a)
    {
      if(enemyCount < maxEnemies)
      {
          enemies[enemyCount] = a;
          enemyCount++;
          totalEnemiesLeft++;
      }
      else
        std::cout << "Enemy Count Limit Reached for this wave." << std::endl;
     
    }
    
    //Robert:: Generates a formation based on wave value in constructor
    void BuildWave()
    {
        switch(waveID)
        {
  //BUILD ENEMY FORMATION HERE
           case 0:                 
              BasicFormation();
                break;
          case 1:   //Andrew Enemy/Bullet patterns
              circleFormation();
              break;
          case 2:
              circleStrategy();
              break;
          case 3:
              yourScrewed();
              break;  //Andrew Enemy/Bullet patterns
          case 4:
              bossLevel();
              break;
          default:
            std::cout <<"Invalid Wave Selected " << waveID << std::endl;
            break;
        }
    }
    
    //Robert:: called when enemy is destroyed to keep track of how many are left
    void UpdateRemainingEnemies()
    {
      totalEnemiesLeft --;
    }
    
    //Robert:: returns how many enemies in current wave remain
    int getRemainingEnemies()
    {
        return totalEnemiesLeft;
    }
     
    
    
    private:
        //Anrew:: Basic Formation with low rewards, but simplistic enemy movements
        void BasicFormation() //Andrew Enemy/Bullet patterns
        { 
             if(enemyCount == maxEnemies)
                  enemyCount == 0;
          
                //parameters
                //(sf::Vector2f position, sf::Vector2f scale, int textureIndex, sf::Texture *bulletT, Bullet **masterBullets, const size_t maxBullets,
                 //sf::VertexArray *verts, int weapon, int hp = 1, int pointValue = 100, size_t *bulletIndex)
                Alien* alien1 = new Alien(sf::Vector2f(0, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien2 = new Alien(sf::Vector2f(200, 100), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 0);
                Alien* alien3 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien4 = new Alien(sf::Vector2f(0, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien5 = new Alien(sf::Vector2f(600, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien6 = new Alien(sf::Vector2f(1400, 300), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 0);
                Alien* alien7 = new Alien(sf::Vector2f(800, 350), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien8 = new Alien(sf::Vector2f(1800, 400), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien9 = new Alien(sf::Vector2f(700, 300), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                  
              
              
                //Alien* alien2 = new Alien(sf::Vector2f(SCREENWIDTH - 30, 400), sf::Vector2f(.05f, .05f), 0, 1, 100);
    
                //set enemy speed and movement direction 
                float a1Speed = 100;
                float a2Speed = 150;
                alien1->setSpeed(a1Speed);
                alien2->setSpeed(a1Speed);
                alien3->setSpeed(a1Speed);
                alien4->setSpeed(a2Speed);
                alien5->setSpeed(a1Speed);
                alien6->setSpeed(a1Speed);
                alien7->setSpeed(a2Speed);
                alien8->setSpeed(a1Speed);
                alien9->setSpeed(a1Speed);
                alien1->setDirection(true);
                alien2->setDirection(true);
                alien3->setDirection(true);
                alien4->setDirection(false);
                alien5->setDirection(false);
                alien6->setDirection(false);
                alien7->setDirection(true);
                alien8->setDirection(true);
                
               
              
                
                addEnemy(alien1);
                addEnemy(alien2);
                addEnemy(alien3);
                addEnemy(alien4);
                addEnemy(alien5);
                addEnemy(alien6);
                addEnemy(alien7);
                addEnemy(alien8);
                addEnemy(alien9);
        } //Andrew Enemy/Bullet patterns
        
        
        //Andrew:: Has several enemies moving in circular motions to be harder to hit
         void circleFormation()  //Andrew Enemy/Bullet patterns
        {
             if(enemyCount == maxEnemies)
                  enemyCount == 0;
          
                //parameters
                //(sf::Vector2f position, sf::Vector2f scale, int textureIndex, sf::Texture *bulletT, Bullet **masterBullets, const size_t maxBullets,
                 //sf::VertexArray *verts, int weapon, int hp = 1, int pointValue = 100, size_t *bulletIndex)
                 
                Alien* alien1 = new Alien(sf::Vector2f(100, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                Alien* alien2 = new Alien(sf::Vector2f(600, 300), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 2);
                Alien* alien3 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                Alien* alien4 = new Alien(sf::Vector2f(0, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien5 = new Alien(sf::Vector2f(900, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 1);
                Alien* alien6 = new Alien(sf::Vector2f(1400, 250), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 1);
                Alien* alien7 = new Alien(sf::Vector2f(300, 200), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 1);
                Alien* alien8 = new Alien(sf::Vector2f(1800, 400), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 0);
                Alien* alien9 = new Alien(sf::Vector2f(700, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                  
              
              
                //Alien* alien2 = new Alien(sf::Vector2f(SCREENWIDTH - 30, 400), sf::Vector2f(.05f, .05f), 0, 1, 100);
    
                //set enemy speed and movement direction 
                float a1Speed = 100;
                float a2Speed = 150;
                alien4->setSpeed(a2Speed);
                alien7->setRadius(3);
                alien8->setSpeed(a1Speed);  
                                
                alien1->setDirection(true);
                alien2->setDirection(true);
                alien3->setDirection(true);
                alien4->setDirection(false);
                alien5->setDirection(false);
                alien6->setDirection(false);
                alien7->setDirection(true);
                alien8->setDirection(true);
                alien9->setDirection(true);                
                
               
              
                
                addEnemy(alien1);
                addEnemy(alien2);
                addEnemy(alien3);
                addEnemy(alien4);
                addEnemy(alien5);
                addEnemy(alien6);
                addEnemy(alien7);
                addEnemy(alien8);
                addEnemy(alien9);
        }//Andrew Enemy/Bullet patterns
        
        
        //Andrew:: Enemies more a bit more furiously, takes more patience to dodge enemy fire
        void circleStrategy()//Andrew Enemy/Bullet patterns
        {
             if(enemyCount == maxEnemies)
                  enemyCount == 0;
          
                //parameters
                //(sf::Vector2f position, sf::Vector2f scale, int textureIndex, sf::Texture *bulletT, Bullet **masterBullets, const size_t maxBullets,
                 //sf::VertexArray *verts, int weapon, int hp = 1, int pointValue = 100, size_t *bulletIndex)
                 
                Alien* alien1 = new Alien(sf::Vector2f(100, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                Alien* alien2 = new Alien(sf::Vector2f(100, 50), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 1);
                Alien* alien3 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 0,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                Alien* alien4 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 1);
                Alien* alien5 = new Alien(sf::Vector2f(900, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 1);
                Alien* alien6 = new Alien(sf::Vector2f(900, 250), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 150, 2);
                Alien* alien7 = new Alien(sf::Vector2f(300, 300), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 1);
                Alien* alien8 = new Alien(sf::Vector2f(300, 300), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                Alien* alien9 = new Alien(sf::Vector2f(700, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 100, 2);
                  
              
              
                //Alien* alien2 = new Alien(sf::Vector2f(SCREENWIDTH - 30, 400), sf::Vector2f(.05f, .05f), 0, 1, 100);
    
                //set enemy speed and movement direction 
                float a1Speed = 100;
                float a2Speed = 150;
                alien4->setSpeed(a2Speed);
                //alien7->setRadius(3);
                alien8->setSpeed(a1Speed);  
                                
//                alien1->setDirection(true);
//                alien2->setDirection(true);
//                alien3->setDirection(true);
//                alien4->setDirection(false);
//                alien5->setDirection(false);
//                alien6->setDirection(false);
//                alien7->setDirection(true);
//                alien8->setDirection(true);
//                alien9->setDirection(true);                
                
               
              
                
                addEnemy(alien1);
                addEnemy(alien2);
                addEnemy(alien3);
                addEnemy(alien4);
                addEnemy(alien5);
                addEnemy(alien6);
                addEnemy(alien7);
                addEnemy(alien8);
                addEnemy(alien9);
        }//Andrew Enemy/Bullet patterns
        
        //Andrew::  High difficulty high reward enemy formation
        void yourScrewed()//Andrew Enemy/Bullet patterns
        {
             if(enemyCount == maxEnemies)
                  enemyCount == 0;
          
                //parameters
                //(sf::Vector2f position, sf::Vector2f scale, int textureIndex, sf::Texture *bulletT, Bullet **masterBullets, const size_t maxBullets,
                 //sf::VertexArray *verts, int weapon, int hp = 1, int pointValue = 100, size_t *bulletIndex)
                Alien* alien1 = new Alien(sf::Vector2f(0, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 0);
                Alien* alien2 = new Alien(sf::Vector2f(200, 100), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 300, 0);
                Alien* alien3 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 0);
                Alien* alien4 = new Alien(sf::Vector2f(0, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 0);
                Alien* alien5 = new Alien(sf::Vector2f(600, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 0);
                Alien* alien6 = new Alien(sf::Vector2f(750, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 2);
                Alien* alien7 = new Alien(sf::Vector2f(750, 50), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 300, 1);
                Alien* alien8 = new Alien(sf::Vector2f(1400, 150), sf::Vector2f(.1f, .1f), 0,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 2);
                Alien* alien9 = new Alien(sf::Vector2f(1400, 150), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 1);
                Alien* alien10 = new Alien(sf::Vector2f(900, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 2);
                Alien* alien11 = new Alien(sf::Vector2f(100, 50), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 2);
                Alien* alien12 = new Alien(sf::Vector2f(600, 300), sf::Vector2f(.1f, .1f), 0,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 300, 2);
                Alien* alien13 = new Alien(sf::Vector2f(400, 150), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 2);
                Alien* alien14 = new Alien(sf::Vector2f(0, 200), sf::Vector2f(.1f, .1f), 1,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 0);
                Alien* alien15 = new Alien(sf::Vector2f(900, 250), sf::Vector2f(.1f, .1f), 1,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 0, 1, 200, 1);  
              
              
                //Alien* alien2 = new Alien(sf::Vector2f(SCREENWIDTH - 30, 400), sf::Vector2f(.05f, .05f), 0, 1, 100);
    
                //set enemy speed and movement direction 
                float a1Speed = 100;
                float a2Speed = 150;
                alien1->setSpeed(a1Speed);
                alien2->setSpeed(a1Speed);
                alien3->setSpeed(a1Speed);
                alien4->setSpeed(a2Speed);
                alien5->setSpeed(a1Speed);

                alien1->setDirection(true);
                alien2->setDirection(true);
                alien3->setDirection(true);
                alien4->setDirection(false);
                alien5->setDirection(false);
                alien11->setDirection(true);
                alien12->setDirection(true);
                alien13->setDirection(true);
                alien14->setDirection(false);
                alien15->setDirection(false);
                
               
              
                
                addEnemy(alien1);
                addEnemy(alien2);
                addEnemy(alien3);
                addEnemy(alien4);
                addEnemy(alien5);
                addEnemy(alien6);
                addEnemy(alien7);
                addEnemy(alien8);
                addEnemy(alien9);
                addEnemy(alien10);
                addEnemy(alien11);
                addEnemy(alien12);
                addEnemy(alien13);
                addEnemy(alien14);
                addEnemy(alien15);
        }//Andrew Enemy/Bullet patterns
    
      //Andrew:: Buils a main boss made up of 5 different pieces, each boss has different firing patterns set to create a truly chaotic level
      void bossLevel()
      {
        
                Alien* alien1 = new Alien(sf::Vector2f(400, -200), sf::Vector2f(1.0f, 1.5f), 5,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 2, 200, 200, 3);
                Alien* alien2 = new Alien(sf::Vector2f(600, -50), sf::Vector2f(1.0f, 1.0f), 3,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 1, 200, 300, 3);
                Alien* alien3 = new Alien(sf::Vector2f(830, -70), sf::Vector2f(1.0f, 1.0f), 2,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 3, 200, 200, 3);
                Alien* alien5 = new Alien(sf::Vector2f(1050, -20), sf::Vector2f(1.0f, 1.0f), 4,  bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 1, 200, 200, 3);
                Alien* alien4 = new Alien(sf::Vector2f(1150, -200), sf::Vector2f(1.0f, 1.5f), 6,   bulletTexture, bullets, maxBullets, vertices, currentBulletIndex, 2, 200, 200, 3);
                addEnemy(alien1);
                addEnemy(alien2);
                addEnemy(alien3);
                addEnemy(alien4);
                addEnemy(alien5);
        
        }
    
    const int SCREENWIDTH = 1920;
    const int SCREENHEIGHT = 1080;
      Alien **enemies;
      size_t maxEnemies;
      int enemyCount = 0;
      
      int totalEnemiesLeft = 0;
      
      
      //bullet system variables
       sf::Texture *bulletTexture; 
       Bullet **bullets;
       size_t *currentBulletIndex; 
       size_t maxBullets; 
       sf::VertexArray *vertices;
       int waveID;
    
    
 
};
#endif