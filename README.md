# Videogames
Videojuegos Unizar


Para compilar el programa se necesita cmake:

mkdir build
cd build
cmake ..
make
./Videogames

-------sfml-----------
g++ -c mainStars.cpp
g++ mainStars.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app