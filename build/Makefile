src=../src
bin=../bin
build=.
CC=g++
std=std=c++11
O=-O3
libsSFML=-lsfml-graphics -lsfml-window -lsfml-system

bomberman: $(src)/bomberman.cpp GameEngine.o
	$(CC) -o bomberman $(src)/bomberman.cpp GameEngine.o $(libsSFML)

GameEngine.o: $(src)/GameEngine.cpp $(src)/GameEngine.hpp
	$(CC) -c $(src)/GameEngine.cpp -o $(build)/GameEngine.o