# HOMM3-mapgen
Template-Based Map Generator for Heroes of Might &amp; Magic III


# Using 
* https://github.com/radekmie/homm3lua/tree/master
* https://github.com/vcmi/vcmi/tree/develop

# Pre requirements 
* Lua5.4, qt5-tools, boost

On Ubuntu you can run
* sudo apt-get install lua5.4 liblua5.4-dev libtbb-dev libsdl2-ttf-dev qttools5-dev libsdl2-mixer-dev libsdl2-image-dev



# Current demo testing
0. Ensure that tou have all of requirements installed

1. Ensure that all submodules are updated
```
git submodule update --init --recursive

```
2. Build local VCMI
```
mkdir build
cd build
cmake -S ../vcmi

cmake --build . -j8
```

Setup vcmi via /bin/vcmilauncher (add game files)

3. make generator & run generator
```
make all
./Generator
```

And checking result
```
vcmieditor
```

Struktura plików:
- gameInfo/ 
    - Faction.h - enum możliwych frakcji w Ho3 + funkcjonalność
    - MineType.h - enum możliwych typów kopalni w Ho3 + funkcjonalność
    - Object.h - Klasa Object podstawowe informacje o obiektach (pozycja, rozmiar)
    - Mine.h - Klasa Mine (wszelkie informacje o kopalniach), kto jest ownerem, jaki to typ, dziedziczy po Object
    - Town.h - Klasa Town (wszelkie informacje o miastach), kto jest ownerem, jakiej frakcji to miasto, dziedziczy po Object
    - Tile.h - Klasa Tile (abstrakcja na pixel), do jakiej zony należy, czy jest na nim border, droga itp
    - Zone.h - Klasa Zone (abstrakcja na Zony), Środek zony, jaki ma mieć teren, jakie obiekty się w tej zonie znajdują
- global/
    - Global.h - jakieś basic using'i (możliwie do wywalenia)
    - Random.h - klasa RNG napisana ala python, by móc np prosto losować z danego przedziału
    - PenroseTiling.h - klasa PenroseTiling używana do zrobienia PenroseTiling
- types/
    - float3.h - klasa float3 (no 3 floaty), można bardziej rozbudować
    - int3.h - klasa int3 (no 3 inty), można bardziej rozbudować
- templateInfo/
    - ConnectionInfo.h - Informacje o połączeniach
    - MineInfo.h - Informacje o kopalniach
    - TemplateInfo.h - Informacje o Templatce
    - TownInfo.h - Informacje o miastach
    - ZoneInfo.h - Informacje o Zonie 
- placers/
    - BorderPlacer.h - Klasa BorderPlacer odpowiedzialna za kładzenie Borderu
    - ObjectPlacer.h - Klasa ObjectPlacer odpowiedzialna za kładzenie obiektów
    - RoadPlacer.h - Klasa RoadPlacer.h odpowiedzialna za kładzenie dróg
    - ZonePlacer.h - Klasa ZonePlacer odpowiedzialna za kładzenie Zone
- .
    - Map.h - Klasa Map (abstrakcja na Mapę, wszelkie informacje tutaj przechowywane mają być odzwierciedleniem prawdziwej mapy), obecnie jest width, height, zony, tiles'y
    - test.cpp [nazwa do zmiany] - Główny program odpowiedzialny za stworzenie mapy (tworzy skrypt Lua który następnie odpala)