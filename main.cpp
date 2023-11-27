#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<string.h>
#include<vector>
#include <cmath>


#include "Ship.cpp"
#include "Alien.cpp"
#include "Bullet.h"
#include "EnemyWave.cpp"
#include "WaveMgr.cpp"

//////MAIN
/////   Contains all game loop logic and intialization of all main actors in the group. All pointers will point to something in here.
//////   Also contains class for power up shop initialization
//////   Authors: Robert, Andrew


//Globabl Constants for window resolution
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;

// Power Up shop that shows up based on a timer. After the shop appears, the next enemy wave to appear is a boss fight
//Author: Andrew
class ShopWindow
{//Andrew-shopUpdate
    
    public:  
    
        //Sets up main show window UI layout
        ShopWindow()
        {
          shop.setSize(sf::Vector2f(1000,600));
          shop.setFillColor(sf::Color(128,128,128,64));
          shop.setPosition(450,240);
          font.loadFromFile("arial.ttf");
          shopText.setString("Shop");
          shopText.setFont(font);
          shopText.setCharacterSize(20);
          shopText.setPosition(950,280);
          choice = -1;
          }
        
        
        //Builds Shop Window elements out for rendering - each item has a window element
        //Param
          // text -- shop window dialog text
          //size - dimensions of shop window
          //position - position of shop window
          //chosen - current element selected
          //cost - how much the powerup costs
        ShopWindow(std::string text,sf::Vector2f size, sf::Vector2f position,int chosen, int cost)
        {
          choice = chosen;
          shop.setSize(size);
          shop.setFillColor(sf::Color(128,128,128,64));
          shop.setPosition(position.x,position.y);
          selected.setSize(size);
          selected.scale(1.05f,1.05f);
          selected.setFillColor(sf::Color(255,255,0,128));
          selected.setPosition(position.x-(shop.getLocalBounds().width*0.025),position.y-(shop.getLocalBounds().height*0.025));
          font.loadFromFile("arial.ttf");
          shopText.setString(text);
          shopText.setFont(font);
          shopText.setCharacterSize(20);
          shopText.setPosition((position.x+(shop.getSize().x/2)-shopText.getLocalBounds().width/2),position.y+40);
          this->cost = cost;

        } 
        
        //returns main shop rectangle UI 
        sf::RectangleShape getShape()
        {
          return this->shop ;         
        }
        
        //returns shopText
        sf::Text getText()
        {
          return this->shopText;
        }
        
        //returns which item is selected in the shop
        sf::RectangleShape getSelect()
        {
          return this->selected;
        }
        
        
        //draw shop elements 
        //Param 
          //window - current application window  that renders everything
        void draw(sf::RenderWindow window)
        {
          window.draw(this->shop);
          window.draw(this->shopText);
        }
        
        //checks if powerup is available for purches - used for one time only purchase powerups
        bool isBuyable(){
        return this->buyable;
        }
        
        
        //Called when user buys a powerup  -- selected item modifies ship stats and reduces score
        //Param:
        /// player --  pointer to ship so it can be modified
        // wallet - pointer to waller so we have points to spend
        void purchase(Ship* player,int* wallet)
        {
          switch (choice)
          {
              case 1:   //Andrew Enemy/Bullet patterns
                 // std::cout << this-> cost << std::endl;
                  if(*wallet >= this->cost)
                  {
                    player->setSpeed(player->getSpeed()+25.0f);
                    *wallet = *wallet - this->cost;
                  }
                  break;
              case 2:
                  if(*wallet >= this->cost)
                  {
                    player->setWeaponPower(player->getWeaponPower()+1);
                    *wallet = *wallet - this->cost;
                    this->buyable = false;
                  }
                  break;
              case 3:
                  if(*wallet >= this->cost){
                    player->setFiringRate(player->getFiringRate()-sf::milliseconds(200));
                    *wallet = *wallet - this->cost;
                    this->buyable=false;
                  }
                  
                  break;
              case 4:
                  if(*wallet >= this->cost){
                    player->setHealth(player->getHealth()+1);
                    *wallet = *wallet - this->cost;
                  }
                 
                  break;
              case 5:
                  if(*wallet >= this->cost){
                    player->changeCurrentWeapon(1);
                    *wallet = *wallet - this->cost;
                  }
                  break;
              case 6:
                  if(*wallet >= this->cost){
                    player->changeCurrentWeapon(2);
                    *wallet = *wallet - this->cost;
                  }
                  break;//Andrew Enemy/Bullet patterns
              default:
                std::cout<<"Not an option"<<std::endl;
                break;
            
          
          }
        
        }
        
    protected:
        sf::RectangleShape shop;   //shop item rectangle
        sf::RectangleShape selected;   //selected item rectangle
        int choice;     //users current choice in shop
        sf::Text shopText;   
        sf::Font font;
        int cost;      //how much score this powerup costs
        bool buyable=true; //Andrew Enemy/Bullet patterns
      
    };//Andrew-shopUpdate

//Contains MAIN GAME loop logic and any intializations of actors in game

int main()
{
    //Robert  - pause menu implementation
    const double Pi = 3.14159265358979323846;
    bool isPaused = false;                      //main pause game variable for whole game loop
    
    //Basic set up of scene and Text: Andrew
    //Setup window and Music components
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Assignment 3");
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(350);  //frame rate takes a bit of a hit with bullets, high cap ensures smoothness
    
   //load Sound Elements :: Robert     
   //Sound Sources: 
           //player explosion : https://pixabay.com/sound-effects/hq-explosion-6288/
           // enemy explosion: https://pixabay.com/sound-effects/explosion-6055/
           // shop background music - https://pixabay.com/music/synthwave-digital-love-127441/
           //mob level background music - https://pixabay.com/music/synthwave-lady-of-the-80x27s-128379/
           // boss level music - https://pixabay.com/music/synthwave-a-hero-of-the-80s-126684/
    
  /////////////                 Background Music
  
    // fader booleans to fade between music sources
    float musicVolume = 50.f;  ///be sure to update in waveMGr class too
    bool fades[6];  // 0 = mobToShop ,, 1 = ShopToBoss, 2 = BossToMob, 3 = shop to mob, 
    fades[0] = false;
    fades[1] = false;
    fades[2] = false;
    fades[3] = false;
    fades[4] = false;
    fades[5] = false;
  
    //all 3 music clips play concurrently, 2 are muted at any one time
    float volumes[3];  
    volumes[0] = musicVolume;
    volumes[1] = 0.f;
    volumes[2] = 0.f;
  
    
    //enemymob level music
     sf::Music mobBg;
     if( !mobBg.openFromFile("mobBg.ogg"))
     {
        std::cout<<"ERROR"<<std::endl;
    }
    mobBg.setLoop(true);
    mobBg.setVolume(musicVolume);      //set volume for background sound 0-100
    mobBg.play();
    
    
    //shop music
     sf::Music shopBg;
     if( !shopBg.openFromFile("shopBg.ogg"))
     {
        std::cout<<"ERROR"<<std::endl;
    }
    shopBg.setLoop(true);
    shopBg.setVolume(0.f);      //set volume for background sound 0-100
    shopBg.play();
    
    //boss music
     sf::Music bossBg;
     if( !bossBg.openFromFile("bossBg.ogg"))
     {
        std::cout<<"ERROR"<<std::endl;
    }
    bossBg.setLoop(true);
    bossBg.setVolume(0.f);      //set volume for background sound 0-100
    bossBg.play();
    
    
    
    
     
  /////////////////////            Sound FX   
    //player hit sound
     sf::SoundBuffer playerFX;
     if( !playerFX.loadFromFile("shipExplosion.ogg"))
     {
        std::cout<<"ERROR"<<std::endl;
    }
    sf::Sound playerHitSound;
    playerHitSound.setBuffer(playerFX);
    playerHitSound.setVolume(100.f);
    
    //enemy defeated sound
     sf::SoundBuffer enemyFX;
     if( !enemyFX.loadFromFile("alienExplosion.ogg"))
     {
        std::cout<<"ERROR"<<std::endl;
    }
    sf::Sound enemyHitSound;
    enemyHitSound.setBuffer(enemyFX);
    enemyHitSound.setVolume(100.f);
    
    
    
 
 //////  SHOP VARIABLES   :: Andrew
     //Level loading variables
     float firstLevelTime= 20.f;                              ///////// Sets how long first level is
    bool shipAppear=false;//Andrew-shopUpdate
    bool shopAppear=false;//Andrew-shopUpdate
    bool shopOver=false;//Andrew-shopUpdate
    int index1=0;//Andrew-shopUpdate
    int index2=0;//Andrew-shopUpdate
    
    ShopWindow shopWindow = ShopWindow(); //Andrew-shopUpdate
    ShopWindow shopArray[2][6]=
     {
      {
            ShopWindow("Upgrade speed\n200",sf::Vector2f(300,200),sf::Vector2f(470,290),1,200),
            ShopWindow("Upgrade damage\nOnly once\n300",sf::Vector2f(300,200),sf::Vector2f(780,290),2,300),
            ShopWindow("Upgrade fire rate\nOnly once\n500",sf::Vector2f(300,200),sf::Vector2f(1090,290),3,500),
            ShopWindow("Upgrade health\n200",sf::Vector2f(300,200),sf::Vector2f(470,500),4,200)
        },
        
        {ShopWindow("Triple shot\n100",sf::Vector2f(300,200),sf::Vector2f(470,290),5,100),
        ShopWindow("fan fire\n1000",sf::Vector2f(300,200),sf::Vector2f(780,290),6,1000)}   //Andrew Enemy/Bullet patterns(Cost updates)
    };
    //Andrew-shopUpdate
    
    
    //Robert : main game GUI elements - score, health
    //setup text for UI elements
    sf::Font font;
    font.loadFromFile("arial.ttf");
    
    //Score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(10, 10);
    scoreText.setFillColor(sf::Color::White);
    
    //health text
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setString("Health: " + std::to_string(3));  //otherwise points to garbage on the first frame
    healthText.setPosition(10, SCREENHEIGHT - 50);
    healthText.setFillColor(sf::Color::White);
    
    
    //Andrew ::shopUpdate shop text 
    sf::Text shopInstructions;
    shopInstructions.setFont(font);
    shopInstructions.setCharacterSize(30);
    shopInstructions.setString("Navigation: Up/Down arrows to change tabs, Left/Right arrows to choose different options.\nBuy:Enter key\nExit Shop: Z key");
    shopInstructions.setPosition(40,SCREENHEIGHT-200);
    shopInstructions.setFillColor(sf::Color::White);
    
    //Robert :: score counter
    int score = 0; // initialize score to zero
    int * scorePT = &score; //Andrew --used in shop
   
    
    	// Robert:: Bullet Setup -- array to store bullets
       //SOURCE: Modified from - https://glusoft.com/tutorials/sfml/creating-bullet-hell-game
	   const size_t maxBullets = 1000; //from a single enemy
	    const size_t totalBullets = 16 * maxBullets;
                   
     size_t currentBulletIndex = 0; //manages which index to make new bullets at across all shooting behaviorsm rewrites the oldest bullets if they are still onscreen
      size_t *bulletIndex = &currentBulletIndex;
 
      //Bullet b;
    	Bullet* bullets[totalBullets]; // = new Bullet[totalBullets];
      for (size_t i = 0; i < totalBullets; i++) 
      {
    		bullets[i] = NULL;
    	}
        
    
      // Use a vertex array for the rendering of the bullets
      sf::VertexArray vertices(sf::Quads, 4 * totalBullets);
      
    	// Load the texture of the bullets
    	sf::Texture bulletTexture; 
    	bulletTexture.loadFromFile("bullets.png");
   
   
     //Robert :: Alien wave system setup  -- runs in a similar manner to bullet management
     const size_t maxEnemies = 20;   //maximum enemies on screen
     Alien *enemies[maxEnemies];
      for (size_t i = 0; i < maxEnemies; i++) 
      {
    		enemies[i] = NULL;
    	}

      //Robert : Wave Manager Initialization - Enemies spawn in groups, called waves, once each wave is deestroyed a new one spawns
      //EnemyWave wave = EnemyWave(enemies, maxEnemies, 0,  &bulletTexture, bullets, maxBullets, &vertices, bulletIndex);
      //wave.BuildWave();
      WaveMgr waveMgr = WaveMgr(enemies, maxEnemies, 0,  &bulletTexture, bullets, maxBullets, &vertices, bulletIndex,musicVolume);
     
     
 
     //Robert : pause menu setup
     sf::Texture pauseTexture;
    sf::Sprite pauseMenu;
    pauseTexture.loadFromFile("pauseScreen.png");
    pauseMenu.setPosition(0, 0);
    pauseMenu.setTexture(pauseTexture);
    pauseMenu.setScale(1.f, 1.f);
    
    //Andrew: backrgound stars setup  -- multiple tiled textures all connected together scroll across the screen and loop after a while
    sf::Texture bgTexture;
    sf::Sprite spaceBG1;
    sf::Sprite spaceBG2;
    bgTexture.loadFromFile("Stars2Back.png");
    spaceBG1.setPosition(0, -564);
    spaceBG1.setTexture(bgTexture);
    spaceBG1.setScale(2.2f, 2.7f);
    spaceBG2.setPosition(0, -2200);
    spaceBG2.setTexture(bgTexture);
    spaceBG2.setScale(2.2f, 2.7f);
    
    //Set up ship and shop assets
    //Robert: Ship Setup
    Ship playerShip(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 100), sf::Vector2f(.1f, .1f), &bulletTexture, bullets, maxBullets, &vertices, 0, bulletIndex);
    playerShip.setSpeed(225.f);
    double velPlayer = 0.2;
    
   
     //Andrew-shopUpdate -- initialize merchant ship
    sf::Texture merchant;
    sf::Sprite merchantShip;
    merchant.loadFromFile("Merchant_ship.png");
    merchantShip.setTexture(merchant);
    merchantShip.setPosition(10,200);
    merchantShip.setScale(1.1f,1.1f);
    
   

  
    //Robert:: setup game time
    float dt = 0;
    float totalTime=0;//Andrew-shopUpdate --counts how much time has passed to spawn in ship
    sf::Clock deltaClock; 
    //game loop
    while (window.isOpen())
    {
        sf::Time timeElapsed = deltaClock.restart();
        dt = timeElapsed.asSeconds();   //get delta time  
  
        
        sf::Event event;
        while (window.pollEvent(event))  //check key events and close events
        {
             //check inputs, commented stuff out is player functionalities not yet implemented
               //Andrew basic key pressed event
               if(event.type == sf::Event::KeyPressed)                  
               {
                    if(event.key.code == sf::Keyboard::Escape)
                   {
                       isPaused = !isPaused;
                   }
                  
                  //Andrew:: Controls when shop is active
                  //changes controls when shop is visible
                   if(shopAppear)
                   {//Andrew-shopUpdate
   
                           if(event.type == sf::Event::KeyPressed)
                           {
                               if (event.key.code == sf::Keyboard::Down)
                               {
                                    if(index2==0) index2=1;
                               }
                               
                               if (event.key.code == sf::Keyboard::Up)
                               {
                                    if(index2==1) index2=0;
                               }
                                    
                               if (event.key.code == sf::Keyboard::Right)
                               {
                                    if(index1!=5) index1++;
                                    else if (index1==5) index1=0;
                               }
                               
                               if (event.key.code == sf::Keyboard::Left)
                               {
                                    if(index1!=0) index1--;
                                    else if (index1==0) index1=5;
                               }
                               
                                if(event.key.code==sf::Keyboard::Enter)//Andrew Enemy/Bullet patterns
                               {
                                 if(shopArray[index2][index1].isBuyable()){
                                 shopArray[index2][index1].purchase(&playerShip,scorePT);
                                 //std::cout<<score<<std::endl;
                                 }                                 
                               }
                            }
                            
                            
                           
                           if(event.key.code ==sf::Keyboard::Z)
                           {
                             shipAppear=false;
                             shopAppear=false;
                             shopOver=true;
                             waveMgr.setBossSpawn(true, &fades[4]);   ///spawn boss wave after shop is closed
                            
                             if(fades[3] == false)
                               fades[3] = true; //fade from shop to level music
                             //shopBg.stop();
                             //bossBg.play();
                             spaceBG1.setPosition(0, -564);
                           }
                   }//Andrew-shopUpdate
                        
               
               }
               
               
        
        
     
         
             //Andrew:: close event handler
            //close window handler
            if (event.type == sf::Event::Closed) 
            {
            
                window.close();
            
            std::cout<<"Event window handled"<<std::endl;
            //exit();
             }
        }
        
                    
///////////////////   MAIN GAME LOOP
  if(!isPaused)  //Robert: Pause Menu checker
  {
        
        totalTime = totalTime+dt; //Andrew:: time counter for shop
       waveMgr.manageMusicFades(fades, volumes, &mobBg, &shopBg, &bossBg); //Robert:: check if any fades are occuring
       //Andrew :: main game loop if shop is not active
       if(!shopAppear)
       {
       
    
              //Robert:: Player movement -- velocity based
              sf::Vector2f dir;
              sf::Vector2f *movementDirection = &dir;  //figure out which direction player wants to move in
             movementDirection->x = 0;
             movementDirection->y = 0;
             //key is pressed
              if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  //up
             {
               
                movementDirection->y = -1;
             }
              else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  //down
             {
                 movementDirection->y = 1;
             }
             
              if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  //left
             {
               movementDirection->x = -1;
             }
             else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //right
             {
               movementDirection->x = 1;
               
             }
             playerShip.Move(movementDirection, dt);  
        
    
              playerShip.manageShipStatus(dt);  //check to see if player is able to be hurt again, if a player is hit, give them a few invincible frames
              
              // Robert:: Update bullets
             
              for (size_t i = 0; i < totalBullets; i++) 
              {
                    //Update positions of all bullets on screen
                   // Source:: https://glusoft.com/tutorials/sfml/creating-bullet-hell-game 
                	if (bullets[i] != NULL) 
                  {
                		bullets[i]->Move(timeElapsed.asMilliseconds());
                		sf::Vertex* quad = &vertices[i * 4];
                		sf::Sprite *spr = bullets[i]->getSprite();
                
                		quad[0].position = sf::Vector2f(spr->getPosition().x, spr->getPosition().y);
                		quad[1].position = sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width,spr->getPosition().y);
                		quad[2].position = sf::Vector2f(spr->getPosition().x + spr->getTextureRect().width,spr->getPosition().y + spr->getTextureRect().height);
                		quad[3].position = sf::Vector2f(spr->getPosition().x, spr->getPosition().y + spr->getTextureRect().height);
                   
              /// Robert:: BULLET COLLISION CHECK
                    if(!  shipAppear)//Andrew: shop check to disable collisions so player doesn't get hurt when shit appears
                    {
                       //check if bullet is player's or enemie's
                       if(bullets[i]->getBulletOwner())
                       {
                             //check if enemy was hit
                               for(size_t j = 0; j < maxEnemies; j++)
                               {
                                   if(enemies[j] != NULL)
                                   {
                                      if(bullets[i]->getSprite()->getGlobalBounds().intersects(enemies[j]->getSprite().getGlobalBounds()))
                                       {
                                          //std::cout << "Hit" << std::endl;
                                      
                                            if(enemies[j]->hit(playerShip.getWeaponPower())) //reduce health by player's weapon power stat
                                            {
                                             
                                                score += enemies[j]->getValue(); //add score
                                                firstLevelTime++;//Andrew:: will give the player a bit more time to shoot down enemies to accumulate more points
                                                delete(enemies[j]); //remove enemy
                                                enemies[j] = NULL;
                                                enemyHitSound.play();  //enemy destroyed sound
                                                
                                                //If boss is defeated, just loops over levels yet again, but longer times between next shop visits
                                                if(waveMgr.updateRemainingEnemies())
                                                {
                                                    waveMgr.resetShopTime(&firstLevelTime, &shipAppear, &shopAppear, &shopOver); //increases length until next shop time
                                                    fades[3] = true;  //load back into mobBG
                                                 
                                                }
                                                  
                                                
                                            }
                                            //removes bullet from list and clears it off the screen
                                           	delete(bullets[i]);
                                             quad[0].position = sf::Vector2f(-1, -1);
                                             quad[1] = sf::Vector2f(-1, -1);
                                             quad[2] = sf::Vector2f(-1, -1);
                                             quad[3]= sf::Vector2f(-1, -1);
                        					          bullets[i] = NULL;
                                            break;  //found a collision now to break before it checks other enemies
                                                                      
                                       }
                                   }
                               } 
                           }
                           else //check bullet collision with player
                           {
                             if(bullets[i] != NULL)
                             {
                                  if(bullets[i]->getSprite()->getGlobalBounds().intersects(playerShip.getSprite().getGlobalBounds()))
                                 {
                                     playerShip.hit(&playerHitSound);
                                    
                                 }
                             }
                              
                               
                           }
                     }
                    
               	  }
            
              }
                
               
              //Robert:: Update Enemies Behaviors
              for (size_t i = 0; i < maxEnemies; i++)
              {
                if(enemies[i] != NULL)
                {
                   
                   enemies[i]->Move(dt); 
                   enemies[i]->updateFireRate(timeElapsed);  
                   
                   if(enemies[i]->checkIfShooting())
                     enemies[i]->shoot(timeElapsed);
                  
                }
                 
              }       
              
             
             
              //Robert:: Player fire
              if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
              {
                  playerShip.shoot(timeElapsed);
                
        		  }
                
            playerShip.updateFireRate(timeElapsed);  //keeps running to see if you can fire weapon in semi-automatic fashion if desired
        
        }
        
      
      //Andrew:: check if the player gets near the merchant ship to enter the store
       if(shipAppear)
        { 
             //Andrew-shopUpdate
               if(playerShip.getSprite().getPosition().x < 400 && playerShip.getSprite().getPosition().y<700)
               {
                 shopAppear=true;
               }//Andrew-shopUpdate
         
        }
                
        
        //Robert:: game closes when health is 0 for player
        if(playerShip.getHealth() <= 0)
        {
            window.close();
        }
          
       
        
        //Robert:: update UI text
        scoreText.setString("Score: " + std::to_string(score));
        
     
        healthText.setString("Health: " + std::to_string(playerShip.getHealth()));
  
  }
  
  
  ///////////// Robert:: Clean up all bullets that are off screen
   for (size_t i = 0; i < totalBullets; i++) 
   {
			if (bullets[i] != NULL) 
      {
				if (bullets[i]->getSprite()->getPosition().x > SCREENWIDTH ||
					bullets[i]->getSprite()->getPosition().y > SCREENHEIGHT ||
					(bullets[i]->getSprite()->getPosition().y + bullets[i]->getSprite()->getTextureRect().height) < 0 ||
					(bullets[i]->getSprite()->getPosition().x + bullets[i]->getSprite()->getTextureRect().width) < 0) {

					delete(bullets[i]);
					bullets[i] = NULL;
				}
			}
		}
    

    
 ///////////////   Window Draw Operations   
    window.clear();
    
    //Andrew:: scroll background if shop is available 
    if(!shopAppear)
    {
        //Draw Space BG ----Andrew
        spaceBG1.setPosition(spaceBG1.getPosition().x,spaceBG1.getPosition().y+1.1f);
        if(spaceBG1.getPosition().y > 1080)
        {
          spaceBG1.setPosition(0,-2200);
          }
        spaceBG2.setPosition(spaceBG2.getPosition().x,spaceBG2.getPosition().y+1.1f);
        if(spaceBG2.getPosition().y > 1080)
        {
          spaceBG2.setPosition(0,-2200);
          }
    }
     else //space does not scroll
     {
          spaceBG1.setPosition(0,-2200);
          spaceBG2.setPosition(0,-2200);
    }
    
    
    //Andrew:: check if shop shows up
    if(totalTime > firstLevelTime  &&  !shopOver)
    { //Andrew-shopUpdate
      shipAppear = true;
//      for (int i = 0;i<maxEnemies;i++){
//        delete(enemies[i]);
//         enemies[i] = NULL;
//      
//      }
      //Robert:: fade music to shop music from whatever is playing
      if(volumes[0] != 0)
      {
           if(fades[0] == false)
              fades[0] = true;    //fade into shop music from level music
      }
      else
      {
           if(fades[5] == false && !fades[4])
              fades[5] = true;    //fade into shop music from boss music
      }
     
   
    } //Andrew-shopUpdate
    
    
    //Andrew:: check if ship is not displayed
    if(!shipAppear)  //draw normal game action
    {
        //Andrew - draw space background
         window.draw(spaceBG1);
          window.draw(spaceBG2);
         
         //Robert draw UI and player
          window.draw(scoreText);
          window.draw(healthText);  
          window.draw(playerShip.getSprite());
       
          
          //Robert:: render all enemies
          for (size_t i = 0; i < maxEnemies; i++)
          {
              if(enemies[i] != NULL)
                window.draw(enemies[i]->getSprite());
          }
          
          //Robert:: render all bullets
          //Source: https://glusoft.com/tutorials/sfml/creating-bullet-hell-game
          sf::Transform transform;
          sf::RenderStates states;
      		states.transform = transform;
      		states.texture = &bulletTexture;  
          window.draw(vertices, states);
    }  
    else   //Andrew:: draw Shop
    {//Andrew-shopUpdate
        window.draw(scoreText);
        window.draw(merchantShip);
        window.draw(playerShip.getSprite());
        if(shopAppear)
        {
                window.draw(shopInstructions);
                window.draw(shopWindow.getShape());
                window.draw(shopWindow.getText());
                window.draw(shopArray[index2][index1].getSelect());
        
                  for(int i =0;i<6;i++)
                  {  
              
                      if(shopArray[index2][i].isBuyable())
                      {
                          //Andrew Enemy/Bullet patterns
                          window.draw(shopArray[index2][i].getShape());
                          window.draw(shopArray[index2][i].getText());
                      }
          
                  }
        }
     }//Andrew-shopUpdate
   
    //Robert:: draw ause menu
    if(isPaused)
      window.draw(pauseMenu);
    window.display();
    }

      //Robert:: bullet clenup before closing application
       for (size_t i = 0; i < totalBullets; i++)
        {
    		    if (bullets[i] != NULL) 
            {
    			      delete(bullets[i]);
    		    }
    	  }
         //enemy cleanup
           for (size_t i = 0; i < maxEnemies; i++)
        {
    		    if (enemies[i] != NULL) 
            {
    			      delete(enemies[i]);
    		    }
    	  }
  
     
     
    return 0;
}