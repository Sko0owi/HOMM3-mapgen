#include "./Object.h"


Object::Object(){
    position = int3(0,0,0);
    name = "";
}

Object::Object(int3 pos, std::string name){
    position = pos;
    this->name = name;
    this->sizeOfObject = int3(1,1,1);
}

Object::Object(int3 pos, std::string name, int3 size){
    this->name = name;
    this->sizeOfObject = size;
    setPosition(pos);
}

int3 Object::getSizeOfObject(){
    return sizeOfObject;
}

int3 Object::getPosition(){
    return position;
}

std::string Object::getName(){
    return name;
}

void Object::setSizeOfObject(int3 size){
    sizeOfObject = size;
}

void Object::setPosition(int3 pos){

    position = pos;
    position.x += sizeOfObject.x / 2;
    position.y += (sizeOfObject.y - 1)/ 2;
}

void Object::setName(std::string name){
    this->name = name;
}

