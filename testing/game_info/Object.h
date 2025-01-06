#pragma once

#include"../types/int3.h"

class Object
{
public:

    Object();
    Object(int3 pos, std::string name);
    Object(int3 pos, std::string name, int3 size);

    int3 getPosition();
    std::string getName();
    int3 getSizeOfObject();


    void setPosition(int3 pos);
    void setName(std::string name);
    void setSizeOfObject(int3 size);


private:
    int3 position; // position on x, y, z axis
    int3 sizeOfObject; 
    std::string name;
};