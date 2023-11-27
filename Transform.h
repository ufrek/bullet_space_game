#include<iostream>
#include "TransformStructs.cpp"


//Robert:: Not used: Reinvented the wheel with position tracking for sprites.....interesting waste of time

class Transform
{
  public:
    Transform(int xVal, int yVal);
     virtual void Move();
     void setPosition(int xVal, int yVal);
     Position getPosition();
     void setScale(float xVal, float yVal);
     Scale getScale();
      void setBounds(int xmin, int xmax, int ymin, int ymax);
       int* getBounds(int output[4]);
};