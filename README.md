# another-HOMM3-mapgen
Template-Based Map Generator for Heroes of Might &amp; Magic III


# Using 
* https://github.com/radekmie/homm3lua/tree/master
* https://github.com/vcmi/vcmi/tree/develop

# Pre requirements 
* Lua5.4
* sudo apt-get install lua5.4 liblua5.4-dev

# Current demo testing
0. Ensure that tou have lua5.4 istalled
1. In testing/test.cpp modify TODO line to save map in desired location 
```
cd testing/
make test
./test
```

And checking result
```
vcmieditor
```

Abstrakcja:
- Template.h
Tutaj ogarniamy wczytywanie JSON'a i przetrzymywanie takich highlevel rzeczy (kto z kim ma być połaczony itp)
- Map.h
Tutaj mają być już dane dokładne gdzie co ma się znajdować. (piksel po pikselu)