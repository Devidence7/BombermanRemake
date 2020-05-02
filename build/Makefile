build=.
CC=g++
std=-std=c++17
O=-O3
PATH := $(PATH):../bin/

src=../src
bin=../bin
libsSFML=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
#libsSFML=-lsfml-all
entityPath=../src/Entities
logicPath=../src/Logic
texturesPath=../src/Textures
interfazPath=../src/Interface
GUIPath=$(interfazPath)/GUI
LayoutPath=$(GUIPath)/Layouts
UtilsInzPath=$(GUIPath)/Utils
MusicPath=$(src)/Music


libs=-L$(bin) -lEntities -lTextures -lGui
libEntities=-L$(bin) -lEntities 

bomberman: $(src)/main.cpp $(bin)/libEntities.a $(bin)/libGui.a $(bin)/libTextures.a $(bin)/Random.o $(bin)/Time.o  $(src)/Include/EntitiesInclude.cpp $(src)/Utils/A_Star.cpp
	$(CC) -o bomberman  $(src)/main.cpp $(src)/Include/EntitiesInclude.cpp $(src)/Utils/A_Star.cpp  $(MusicPath)/GameMusic.cpp $(MusicPath)/GameSounds.cpp  \
		$(bin)/Random.o $(bin)/Time.o  \
		$(bin)/Arrow.o $(bin)/Box.o $(bin)/Cross.o $(UtilsInzPath)/ItemBox.hpp \
		$(bin)/FormLayout.o $(bin)/HorizontalBoxLayout.o $(bin)/Layout.o $(bin)/Menu.o $(bin)/VerticalBoxLayout.o \
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

$(bin)/libTextures.a: $(bin)/TextureVector.o $(bin)/BombTexture.o $(bin)/EnemyTexture.o $(bin)/FireTexture.o $(bin)/PowerUpTexture.o $(bin)/TextureStorage.o $(bin)/WallTexture.o $(bin)/PlayerTexture.o
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


##################################
#	Compile Interfaze 			#
#################################

#Lib Music


## Lib GUI
$(bin)/libGui.a: $(bin)/Button.o $(bin)/CheckBox.o $(GUIPath)/ComboBox.inl $(GUIPath)/ComboBox.hpp $(bin)/GameGUI.o $(bin)/Image.o $(bin)/InputBox.o $(bin)/Label.o $(bin)/MultilineInputBox.o $(GUIPath)/OptionsBox.inl $(GUIPath)/OptionsBox.hpp $(bin)/ProgressBar.o $(bin)/ProgressBar.o $(bin)/Slider.o $(bin)/SpriteButton.o $(bin)/Theme.o $(bin)/Widget.o
	ar rc $(bin)/libGui.a  $(bin)/libLayout.a $(bin)/libUtilsInterfaze.a $(bin)/Button.o $(bin)/CheckBox.o $(GUIPath)/ComboBox.inl $(bin)/GameGUI.o $(bin)/Image.o $(bin)/InputBox.o $(bin)/Label.o $(bin)/MultilineInputBox.o $(GUIPath)/OptionsBox.inl  $(bin)/ProgressBar.o $(bin)/ProgressBar.o $(bin)/Slider.o $(bin)/SpriteButton.o $(bin)/Theme.o $(bin)/Widget.o
	ranlib $(bin)/libGui.a


$(bin)/Button.o: $(GUIPath)/Button.cpp $(GUIPath)/Button.hpp
	$(CC) -o $(bin)/Button.o -c $(GUIPath)/Button.cpp  $(std) $(O)

$(bin)/CheckBox.o: $(GUIPath)/CheckBox.cpp $(GUIPath)/CheckBox.hpp
	$(CC) -o $(bin)/CheckBox.o -c $(GUIPath)/CheckBox.cpp  $(std) $(O)

#$(bin)/ComboBox.o: $(GUIPath)/ComboBox.inl $(GUIPath)/ComboBox.hpp
#	$(CC) -o $(bin)/ComboBox.o -c $(GUIPath)/ComboBox.inl  $(std) $(O)

$(bin)/GameGUI.o: $(GUIPath)/GameGUI.cpp $(GUIPath)/GameGUI.hpp
	$(CC) -o $(bin)/GameGUI.o -c $(GUIPath)/GameGUI.cpp  $(std) $(O)

$(bin)/Image.o: $(GUIPath)/Image.cpp $(GUIPath)/Image.hpp
	$(CC) -o $(bin)/Image.o -c $(GUIPath)/Image.cpp  $(std) $(O)

$(bin)/InputBox.o: $(GUIPath)/InputBox.cpp $(GUIPath)/InputBox.hpp
	$(CC) -o $(bin)/InputBox.o -c $(GUIPath)/InputBox.cpp  $(std) $(O)

$(bin)/Label.o: $(GUIPath)/Label.cpp $(GUIPath)/Label.hpp
	$(CC) -o $(bin)/Label.o -c $(GUIPath)/Label.cpp  $(std) $(O)
	
$(bin)/MultilineInputBox.o: $(GUIPath)/MultilineInputBox.cpp $(GUIPath)/MultilineInputBox.hpp
	$(CC) -o $(bin)/MultilineInputBox.o -c $(GUIPath)/MultilineInputBox.cpp  $(std) $(O)  $(libsSFML)

#$(bin)/OptionsBox.o: $(GUIPath)/OptionsBox.cpp $(GUIPath)/OptionsBox.hpp
#	$(CC) -o $(bin)/OptionsBox.o -c $(GUIPath)/OptionsBox.cpp  $(std) $(O)
	
$(bin)/ProgressBar.o: $(GUIPath)/ProgressBar.cpp $(GUIPath)/ProgressBar.hpp
	$(CC) -o $(bin)/ProgressBar.o -c $(GUIPath)/ProgressBar.cpp  $(std) $(O)

$(bin)/Slider.o: $(GUIPath)/Slider.cpp $(GUIPath)/Slider.hpp
	$(CC) -o $(bin)/Slider.o -c $(GUIPath)/Slider.cpp  $(std) $(O)

$(bin)/SpriteButton.o: $(GUIPath)/SpriteButton.cpp $(GUIPath)/SpriteButton.hpp
	$(CC) -o $(bin)/SpriteButton.o -c $(GUIPath)/SpriteButton.cpp  $(std) $(O)

$(bin)/Theme.o: $(GUIPath)/Theme.cpp $(GUIPath)/Theme.hpp
	$(CC) -o $(bin)/Theme.o -c $(GUIPath)/Theme.cpp  $(std) $(O)

$(bin)/Widget.o: $(GUIPath)/Widget.cpp $(GUIPath)/Widget.hpp
	$(CC) -o $(bin)/Widget.o -c $(GUIPath)/Widget.cpp  $(std) $(O)
	

## Lib Layouts

$(bin)/libLayout.a:  $(bin)/FormLayout.o $(bin)/HorizontalBoxLayout.o $(bin)/Layout.o $(bin)/Menu.o $(bin)/VerticalBoxLayout.o
	ar rc $(bin)/libLayout.a $(bin)/FormLayout.o $(bin)/HorizontalBoxLayout.o $(bin)/Layout.o $(bin)/Menu.o $(bin)/VerticalBoxLayout.o
	ranlib $(bin)/libLayout.a
 
$(bin)/FormLayout.o: $(LayoutPath)/FormLayout.cpp $(LayoutPath)/FormLayout.hpp
	$(CC) -o $(bin)/FormLayout.o -c $(LayoutPath)/FormLayout.cpp  $(std) $(O)

$(bin)/HorizontalBoxLayout.o: $(LayoutPath)/HorizontalBoxLayout.cpp $(LayoutPath)/HorizontalBoxLayout.hpp
	$(CC) -o $(bin)/HorizontalBoxLayout.o -c $(LayoutPath)/HorizontalBoxLayout.cpp  $(std) $(O)

$(bin)/Layout.o: $(LayoutPath)/Layout.cpp $(LayoutPath)/Layout.hpp
	$(CC) -o $(bin)/Layout.o -c $(LayoutPath)/Layout.cpp  $(std) $(O)

$(bin)/Menu.o: $(LayoutPath)/Menu.cpp $(LayoutPath)/Menu.hpp
	$(CC) -o $(bin)/Menu.o -c $(LayoutPath)/Menu.cpp  $(std) $(O)

$(bin)/VerticalBoxLayout.o: $(LayoutPath)/VerticalBoxLayout.cpp $(LayoutPath)/VerticalBoxLayout.hpp
	$(CC) -o $(bin)/VerticalBoxLayout.o -c $(LayoutPath)/VerticalBoxLayout.cpp  $(std) $(O)


## Lib Utils Interfaze

$(bin)/libUtilsInterfaze.a:  $(bin)/Arrow.o $(bin)/Box.o $(bin)/Cross.o $(UtilsInzPath)/ItemBox.inl $(UtilsInzPath)/ItemBox.hpp
	ar rc $(bin)/libUtilsInterfaze.a $(bin)/Arrow.o $(bin)/Box.o $(bin)/Cross.o $(UtilsInzPath)/ItemBox.inl 
	ranlib $(bin)/libUtilsInterfaze.a


$(bin)/Arrow.o: $(UtilsInzPath)/Arrow.cpp $(UtilsInzPath)/Arrow.hpp
	$(CC) -o $(bin)/Arrow.o -c $(UtilsInzPath)/Arrow.cpp  $(std) $(O)

$(bin)/Box.o: $(UtilsInzPath)/Box.cpp $(UtilsInzPath)/Box.hpp
	$(CC) -o $(bin)/Box.o -c $(UtilsInzPath)/Box.cpp  $(std) $(O)

$(bin)/Cross.o: $(UtilsInzPath)/Cross.cpp $(UtilsInzPath)/Cross.hpp
	$(CC) -o $(bin)/Cross.o -c $(UtilsInzPath)/Cross.cpp  $(std) $(O)

#$(bin)/ItemBox.o: 
#	$(CC) -o $(bin)/ItemBox.o -c $(UtilsInzPath)/ItemBox.inl  $(std) $(O)

