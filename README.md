# another-HOMM3-mapgen
Template-Based Map Generator for Heroes of Might &amp; Magic III


# Using 
* https://github.com/radekmie/homm3lua/tree/master
* https://github.com/vcmi/vcmi/tree/develop

# Pre requirements 
* Lua5.4
* sudo apt-get install lua5.4 liblua5.4-dev

# Current demo testing
0. Ensure that tou have lua5.4 installed
1. Build local VCMI
```
mkdir build
cd build
cmake -S ../vcmi

cmake --build . -j8
```

2. In testing/test.cpp modify TODO line to save map in desired location 
```
cd testing/
make test
./test
```

And checking result
```
vcmieditor
```

Struktura plików:
- game_info/ 
    - Faction.h - enum możliwych frakcji w Ho3 + funkcjonalność
    - Tile.h - Klasa Tile (abstrakcja na pixel)
    - Town.h - Klasa Town (wszelkie informacje o mieście) na razie tylko do jakiej frakcji należy
    - Zone.h - Klasa Zone (abstrakcja na Zony), Środek zony, jaki ma mieć teren, kto jest jej ownerem (może być nikt)
- global/
    - Global.h - jakieś basic using'i (możliwie do wywalenia)
    - Random.h - klasa RNG napisana ala python, by móc np prosto losować z danego przedziału
- types/
    - float3.h - klasa float3 (no 3 floaty), można bardziej rozbudować
    - int3.h - klasa int3 (no 3 inty), można bardziej rozbudować
- .
    - Map.h - Klasa Map (abstrakcja na Mapę, wszelkie informacje tutaj przechowywane mają być odzwierciedleniem prawdziwej mapy), obecnie jest width, height, zony, tiles'y
    - Template.h - Klasa TemplateInfo (abstrakcja na dane wczytane z configa), klasa powstała jako osobny byt, by deserializować json'a i mieć swego rodzaju pomost pomiędzy configiem a klasą Map
    - ZonePlacer.h - Klasa Placer (abstrakcja na tworzenie Zony w mapie), ma funkcję generateZones, która za wszystko już odpowiada

BUG? 
Cant place town without owner?