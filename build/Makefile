src=../src
bin=../bin
build=.
CC=g++
std=-std=c++11
O=-O3
libsSFML=-lsfml-graphics -lsfml-window -lsfml-system

bomberman: $(src)/main.cpp 
	$(CC) -o bomberman $(src)/main.cpp  $(std) $(libsSFML)

GameEngine.o: $(src)/GameEngine.cpp $(src)/GameEngine.hpp
	$(CC) -c $(src)/GameEngine.cpp -o $(build)/GameEngine.o $(std)