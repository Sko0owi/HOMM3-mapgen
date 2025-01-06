#pragma once

#include"../types/int3.h"

class Object
{
public:

    Object();
    Object(int3 pos, std::string name);

    int3 getPosition();
    std::string getName();


    void setPosition(int3 pos);
    void setName(std::string name);


private:
    int3 position; // position on x, y, z axis
    std::string name;
};