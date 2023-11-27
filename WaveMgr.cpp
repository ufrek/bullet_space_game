#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time() random seeding
#include <vector>

#include "Bullet.h"
#include "BulletPattern.cpp"
#include "Alien.cpp"
#include "EnemyWave.cpp"

//Basically the level selector in the game/ randomly loads up different enemy formations when all enemies of current wave are destroyed
//Calls on boss formation to be next when store appears


#ifndef _WaveMgr_
#define _WaveMgr_
class WaveMgr
{
  public:
    //Robert:: Initializes all enemy formation types into a single container and passed all master variables to each instance, handles fading of music between waves, shop, and boss levels
    //Param
      //enemiesArr - main container for all enemies in the game
      //maxE - maximum enemies displayed on screen at once
      //bulletT - main sprite sheet of bullets
      //masterBullets - main container for all bullets in gam
      //maxB - maximum bullets displayed onscreen at once
      //verts - main vertex array for rendering all bullets on screen
      //bulletIndex - current slot at which net bullet should be made in main bllet array
      //max
    WaveMgr(Alien **enemiesArr, const size_t maxE, int wave,  sf::Texture *bulletT, Bullet **masterBullets, const size_t maxB, sf::VertexArray *verts, size_t *bulletIndex, float maxMusicVolume)
    {
        this ->maxMusicVol = maxMusicVolume;
        
       // EnemyWave bossWave = EnemyWave()
        
        //make a vector of 10 waves + 1 boss wave at the end
        waves.push_back (EnemyWave(enemiesArr, maxE, 0,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 0,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 0,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 1,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 1,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 1,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 2,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 2,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 3,  bulletT, masterBullets, maxB, verts, bulletIndex));
        waves.push_back (EnemyWave(enemiesArr, maxE, 3,  bulletT, masterBullets, maxB, verts, bulletIndex));
        
        waves.push_back (EnemyWave(enemiesArr, maxE, 4,  bulletT, masterBullets, maxB, verts, bulletIndex));
        
        selectWave();
        
        //std::cout<<"made wave" << std::endl;
    }
    
    //Robert:: either randomly selects a wave or sets the boss wave to be next
    void selectWave()
    {
      //if done shopping, spawn the boss on the next wave  
      if(!spawnBoss)
      {
             currentWave =  (float)(rand() % waves.size() - 1 );
      }
      else
      {
        if(bossMusicOn != NULL)
          *bossMusicOn = true;
        
        currentWave = waves.size() - 1;
        spawnBoss = false;
         
        
          
      }

      waves[currentWave].BuildWave();
   
    }
    
    //Robert:: if all enemies are destroyed in current wave, randomy spawn a new formation of enemies from container
      bool  updateRemainingEnemies()
    {
      //decrements remaining enemies in wave. if it hits 0, build a new random wave
       waves[currentWave].UpdateRemainingEnemies();
       
       if(waves[currentWave].getRemainingEnemies() <= 0)
       {
          
          
           
            if(currentWave == waves.size() - 1)  //if boss, rreturn true to reset level variables
           {
                selectWave();
                return true;
           }
           else
           {
               selectWave();
               return false;
           }
       } 
       return false;
       
    }
    
    //Robert:: reset the shop's spawn time after a successful boss fight
    //Param
      //timer - how much time level has before we visit the shop again
      //shipAppear - if the ship is displayed on the screen
      //shopAppear - if the shop si displayed on the screen
      //shopOver - true when we exit the shop, we summon the boss wave nextby checking this
    void resetShopTime(float *timer, bool *shipAppear, bool *shopAppear, bool *shopOver)
    {  
        *shipAppear = false;
        *shopAppear = false;
        *shopOver = false;
        levelTime += 10.f;
        *timer = levelTime;
    }
    
    
    //Robert:: coordinates all fades from one music source to another
    //Param
      //fades - each bool tells us if we should initiate a transition betwene two pieces of music and which 2 pieces are involved
      //volumes - volume levels for each concurrently played song in the soundtrack
      //mobBg - pointer to music being played during levels where lots of small ships are on the screen
      //shopBg - pointer to shop music to play
      //bossBg - music to play when the boss wave starts
    void manageMusicFades(bool (&fades)[6], float (&volumes)[3], sf::Music *mobBg, sf::Music *shopBg, sf::Music *bossBg )
    {
        
    
        if(fades[0])
        {
           if(volumes[0] > 0.f)
           {
               volumes[0] -= fadeSpeed;
              
               if(volumes[1] < maxMusicVol)
                volumes[1] += fadeSpeed;
               else
                 volumes[1] = maxMusicVol;
           } 
           else if(volumes[0] < 0.f)
             volumes[0] = 0.f;    
            
            
            
            mobBg->setVolume(volumes[0]);
            shopBg->setVolume(volumes[1]);
            
            
            
            if(volumes[0] == 0.f)
            {
                 mobBg->setVolume(0.f);
                fades[0] = false;
            }
            
            
        }
        else if(fades[1])
        {
            if(volumes[1] > 0.f)
           {
               volumes[1] -= fadeSpeed;
               
               if(volumes[2] < maxMusicVol)
                volumes[2] += fadeSpeed;
               else
                 volumes[2] = maxMusicVol;
           } 
           else if(volumes[1] < 0.f)
             volumes[1] = 0.f;    
            
            shopBg->setVolume(volumes[1]);
            bossBg->setVolume(volumes[2]);
            
            if(volumes[1] == 0.f)
            {
                 shopBg->setVolume(0.f);
                fades[1] = false;
            }
        
        }
        else if(fades[2])  //boss to lvl music
        {
           if(volumes[2] > 0.f)
           {
               volumes[2] -= fadeSpeed;
               
               if(volumes[0] < maxMusicVol)
                volumes[0] += fadeSpeed;
               else
                 volumes[0] = maxMusicVol;
           } 
           else if(volumes[2] < 0.f)
           {
             volumes[2] = 0.f;  
       
           }
             
            
            bossBg->setVolume(volumes[2]);
            mobBg->setVolume(volumes[0]);
            
            if(volumes[2] == 0.f)
            {
                bossBg->setVolume(0.f);
                fades[2] = false;
            }
        
        }
        else if(fades[3])  //from shop to level music
        {
               if(volumes[1] > 0.f)
           {
               volumes[1] -= fadeSpeed;
               
               if(volumes[0] < maxMusicVol)
                volumes[0] += fadeSpeed;
               else
                 volumes[0] = maxMusicVol;
           } 
           else if(volumes[1] < 0.f)
             volumes[1] = 0.f;    
            
            shopBg->setVolume(volumes[1]);
            mobBg->setVolume(volumes[0]);
            
            if(volumes[1] == 0.f)
            {
                 shopBg->setVolume(0.f);
                fades[3] = false;
            }
        
            
        }
        else if(fades[4]) //from level to boss music
        {
            
                 if(volumes[0] > 0.f)
                 {
                     volumes[0] -= fadeSpeed;
                     
                     if(volumes[2] < maxMusicVol)
                      volumes[2] += fadeSpeed;
                     else
                       volumes[2] = maxMusicVol;
                 } 
                 else if(volumes[0] < 0.f)
                   volumes[0] = 0.f;    
                  
                  
                  
                  mobBg->setVolume(volumes[0]);
                  bossBg->setVolume(volumes[2]);
                  
                  
                  
                  if(volumes[0] == 0.f)
                  {
                       mobBg->setVolume(0.f);
                      fades[4] = false;
                  }
            
        
        }
        
         else if(fades[5])  //boss to shop music
        {
           if(volumes[2] > 0.f)
           {
               volumes[2] -= fadeSpeed;
               
               if(volumes[1] < maxMusicVol)
                volumes[1] += fadeSpeed;
               else
                 volumes[1] = maxMusicVol;
           } 
           else if(volumes[2] < 0.f)
           {
             volumes[2] = 0.f;  
       
           }
             
            
            bossBg->setVolume(volumes[2]);
            shopBg->setVolume(volumes[1]);
            
            if(volumes[2] == 0.f)
            {
                bossBg->setVolume(0.f);
                fades[5] = false;
            }
        
        }
    }
    
    
    //Robert:: when exitting shop we trigger the boss wave to spawn next
    //Param
      //b - determines wheter the boos will apper
      //bossMusicFader - determines which fade event to trigger - in thi case from mob level music to boss music - can alter which boss is going to be played here if implemented
    void setBossSpawn(bool b, bool *bossMusicFader)
    {
        bossMusicOn = bossMusicFader;
        spawnBoss = b;
    }
    
    private:
      std::vector<EnemyWave> waves;
      int totalWaves;
      int currentWave;
      bool spawnBoss = false;
      int levelTime = 30;
      
      float maxMusicVol = 50.f;
      bool *bossMusicOn = NULL;
      float fadeSpeed = .1f;
      

};
#endif