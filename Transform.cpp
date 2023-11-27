#include<iostream>
#include "TransformStructs.cpp"

#ifndef _Transform_
#define _Transform_

//Robert:: Not used: Reinvented the wheel with position tracking for sprites.....interesting waste of time

class Transform
{
  public:
    Transform(int xVal, int yVal) { pos.x = xVal; pos.y = yVal; scale.x = 1.0f; scale.y = 1.0f; }
    virtual void Move() = 0;   //implemented for each player/enemy
    void setPosition(int xVal, int yVal)
    {
      pos.x = xVal;
      pos.y = yVal;
    }
    
    Position getPosition()
    {
        //int position[2] {pos.x, pos.y};
        return pos;
    }
    
    void setScale(float xVal, float yVal)
    {
      scale.x = xVal;
      scale.y = yVal;
    }
    Scale getScale()
    {
      return scale;
    }
    void setBounds(int xmin, int xmax, int ymin, int ymax) 
     {
       xMin = xmin;
       xMax = xmax;
       yMin = ymax;
       yMax = ymin;
     }
     int* getBounds(int output[4])
     {
       output[0] = xMin;
       output[1] = xMax;
       output[2] = yMin;
       output[3] = yMax;
       return output;
     }
  
    
  protected:
    Position pos;
    Scale scale;
    int xMin, xMax, yMin, yMax;
    
};

#endif