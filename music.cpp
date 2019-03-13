#include "music.h"

const bool loadMusic(sf::Music& music,const std::string& fileName){
	return music.openFromFile("resources/music/"+fileName);
}

void playMusic(sf::Music& music,const std::string& fileName){
	if(loadMusic(music,fileName)){
		music.play();
	}
}