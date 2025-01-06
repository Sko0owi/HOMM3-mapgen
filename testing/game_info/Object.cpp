#include "./Object.h"


Object::Object(){
    position = int3(0,0,0);
    name = "";
}

Object::Object(int3 pos, std::string name){
    position = pos;
    this->name = name;
}

int3 Object::getPosition(){
    return position;
}

std::string Object::getName(){
    return name;
}

void Object::setPosition(int3 pos){
    position = pos;
}

void Object::setName(std::string name){
    this->name = name;
}

