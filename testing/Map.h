#include<bits/stdc++.h>
#include "./types/int3.h"

class Zone;

class Map {
public:
    Map(int width, int height);
    ~Map();
    void generate();
    void print();
private:
    int width;
    int height;
    std::map<i32, std::shared_ptr<Zone>> zones;

};