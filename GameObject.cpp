#include<iostream>
#include "Transform.cpp"

#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_
//UNUSED: used to hold any other general purpose things that are thought of, might not be needed for this assignment
//Robert
class GameObject : public Transform
{
   public:

        GameObject(int xVal, int yVal)
        : Transform(xVal, yVal)
        {
        
        }
};

#endif