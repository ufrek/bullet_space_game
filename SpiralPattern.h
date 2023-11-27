#include<iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Bullet.cpp"
#include "BulletPattern.cpp"

/// Robert :: NOT USED.....Can't figure out proper inheritance structure

class SpiralPattern : BulletPattern
{
    SpiralPattern(sf::Sprite *id, sf::Texture* texture,  Bullet **masterBulletArr[], const size_t maxBull,  sf::IntRect bulletImg, sf::VertexArray &verts);
     void shoot(float deltaTime);
     SpiralPattern& operator=(SpiralPattern const& other);
    
};

