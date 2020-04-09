build=.
CC=g++
std=-std=c++17
O=-O3
PATH := $(PATH):../bin/

src=../src
bin=../bin
libsSFML=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
entityPath=../src/Entities
logicPath=../src/Logic
texturesPath=../src/Textures

libs=-L$(bin) -lEntities -lTextures
libEntities=-L$(bin) -lEntities 

bomberman: $(src)/main.cpp $(bin)/libEntities.a $(bin)/libTextures.a $(bin)/Random.o $(bin)/Time.o $(bin)/EntitiesInclude.o $(src)/Include/EntitiesInclude.cpp $(src)/Utils/A_Star.cpp
	$(CC) -o bomberman  $(src)/main.cpp $(src)/Include/EntitiesInclude.cpp $(src)/Utils/A_Star.cpp \
		$(bin)/Random.o $(bin)/Time.o  \
		$(libs) \
		$(std) $(libsSFML) $(O) 

GameEngine.o: $(src)/GameEngine.cpp $(src)/GameEngine.hpp
	$(CC) -c $(src)/GameEngine.cpp -o $(build)/GameEngine.o $(std)


clear: 
	rm bomberman || true
	rm $(bin)/*.o  || true
	rm $(bin)/*.a || true
	rm *.o || true

##################################
#	Compile Utils 			#
#################################

$(bin)/Random.o: $(logicPath)/Random.cpp $(logicPath)/Random.h
	$(CC) -o $(bin)/Random.o -c $(logicPath)/Random.cpp  $(std) $(O)

$(bin)/Time.o: $(logicPath)/Time.cpp $(logicPath)/Time.h
	$(CC) -o $(bin)/Time.o -c $(logicPath)/Time.cpp  $(std) $(O)

$(bin)/EntitiesInclude.o: $(src)/Include/EntitiesInclude.hpp $(src)/Include/EntitiesInclude.cpp
	$(CC) -o $(bin)/EntitiesInclude.o -c $(src)/Include/EntitiesInclude.cpp  $(std) $(O)



##################################
#	Compile Entities 			#
#################################


../bin/libEntities.a: ../bin/Entity.o  ../bin/Bomb.o  ../bin/BrickWall.o ../bin/Enemy.o ../bin/Pillar.o  ../bin/PowerUp.o  ../bin/Player.o
	ar rc $(bin)/libEntities.a $(bin)/Entity.o $(bin)/Bomb.o $(bin)/BrickWall.o $(bin)/Enemy.o $(bin)/Pillar.o $(bin)/PowerUp.o $(bin)/Player.o
	ranlib $(bin)/libEntities.a

../bin/Entity.o:  $(entityPath)/Entity.h $(entityPath)/Entity.cpp
	$(CC) -o $(bin)/Entity.o -c $(entityPath)/Entity.cpp $(O) $(std)

../bin/Bomb.o: $(entityPath)/Bomb.cpp $(entityPath)/Bomb.h 
	$(CC) -o $(bin)/Bomb.o -c $(entityPath)/Bomb.cpp $(std) $(O)

../bin/BrickWall.o: $(entityPath)/BrickWall.cpp $(entityPath)/BrickWall.h 
	$(CC) -o $(bin)/BrickWall.o -c $(entityPath)/BrickWall.cpp $(std) $(O)

../bin/Enemy.o: $(entityPath)/Enemy.cpp $(entityPath)/Enemy.h
	$(CC) -o $(bin)/Enemy.o -c $(entityPath)/Enemy.cpp $(std) $(O)

../bin/Pillar.o: $(entityPath)/Pillar.cpp $(entityPath)/Pillar.hpp 
	$(CC) -o $(bin)/Pillar.o -c $(entityPath)/Pillar.cpp $(std) $(O)

../bin/Player.o: $(entityPath)/Player.cpp $(entityPath)/Player.h 
	$(CC) -o $(bin)/Player.o -c $(entityPath)/Player.cpp $(std)  $(O)

../bin/PowerUp.o: $(entityPath)/PowerUp.cpp $(entityPath)/PowerUp.h 
	$(CC) -o $(bin)/PowerUp.o -c $(entityPath)/PowerUp.cpp  $(std) $(O)


##################################
#	Compile Textures 			#
#################################

$(bin)/libTextures.a:  $(bin)/TextureVector.o $(bin)/BombTexture.o $(bin)/EnemyTexture.o $(bin)/FireTexture.o $(bin)/PowerUpTexture.o $(bin)/TextureStorage.o $(bin)/WallTexture.o $(bin)/PlayerTexture.o
	ar rc $(bin)/libTextures.a  $(bin)/TextureVector.o $(bin)/BombTexture.o $(bin)/EnemyTexture.o $(bin)/FireTexture.o $(bin)/PlayerTexture.o $(bin)/PowerUpTexture.o $(bin)/TextureStorage.o $(bin)/WallTexture.o
	ranlib $(bin)/libTextures.a

$(bin)/TextureVector.o: $(texturesPath)/TextureVector.cpp $(texturesPath)/TextureVector.h
	$(CC) -o $(bin)/TextureVector.o -c $(texturesPath)/TextureVector.cpp  $(std) $(O)

$(bin)/BombTexture.o: $(texturesPath)/BombTexture.cpp $(texturesPath)/BombTexture.h
	$(CC) -o $(bin)/BombTexture.o -c $(texturesPath)/BombTexture.cpp  $(std) $(O)

$(bin)/EnemyTexture.o: $(texturesPath)/EnemyTexture.cpp $(texturesPath)/EnemyTexture.h
	$(CC) -o $(bin)/EnemyTexture.o -c $(texturesPath)/EnemyTexture.cpp  $(std) $(O)

$(bin)/FireTexture.o: $(texturesPath)/FireTexture.cpp $(texturesPath)/FireTexture.h
	$(CC) -o $(bin)/FireTexture.o -c $(texturesPath)/FireTexture.cpp  $(std) $(O)

$(bin)/PlayerTexture.o: $(texturesPath)/PlayerTexture.cpp $(texturesPath)/PlayerTexture.hpp
	$(CC) -o $(bin)/PlayerTexture.o -c $(texturesPath)/PlayerTexture.cpp  $(std) $(O)

$(bin)/PowerUpTexture.o: $(texturesPath)/PowerUpTexture.cpp $(texturesPath)/PowerUpTexture.h
	$(CC) -o $(bin)/PowerUpTexture.o -c $(texturesPath)/PowerUpTexture.cpp  $(std) $(O)

$(bin)/TextureStorage.o: $(texturesPath)/TextureStorage.cpp $(texturesPath)/TextureStorage.h
	$(CC) -o $(bin)/TextureStorage.o -c $(texturesPath)/TextureStorage.cpp  $(std) $(O)

$(bin)/WallTexture.o: $(texturesPath)/WallTexture.cpp $(texturesPath)/WallTexture.h
	$(CC) -o $(bin)/WallTexture.o -c $(texturesPath)/WallTexture.cpp  $(std) $(O)


