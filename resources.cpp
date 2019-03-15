#include "resources.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "utility.h"

struct SpriteSheetResource{
	std::string imageName;
	std::vector<int> configuration;
	SpriteSheetResource(std::string imageName,std::vector<int> configuration):imageName(imageName),configuration(configuration){}
};

struct TileMapResource{
	std::string name;
	resource::SpriteSheet spriteSheet;
	sf::Vector2u size;
	sf::Vector2u tileSize;
	int tileCount;
	std::vector<int> solidTiles;

	TileMapResource::TileMapResource(std::string name, resource::SpriteSheet spriteSheet,const sf::Vector2u size, const sf::Vector2u tileSize, const std::vector<int> solidTiles) :
		name(name), spriteSheet(spriteSheet), size(size), tileSize(tileSize), tileCount(0), solidTiles(solidTiles){}
};

std::vector<SpriteSheetResource> spriteSheetResources = {
	SpriteSheetResource("spritesheet_example.png",{6,5}),
	SpriteSheetResource("spritesheet_example2.png",{6,4}),
	SpriteSheetResource("spritesheet_example3.png",{8,1}),
	SpriteSheetResource("levels_tileset.png",{5,2}),
	SpriteSheetResource("rough_spritesheet.png",{6,5}),
	SpriteSheetResource("irregular_spritesheet.png",{0,0,64,64,64,0,128,64,192,0,64,128,0,64,192,64})
};

std::vector<Animation> animationResources = {
	Animation(0.8,{0,1,2,3,4,5,6,7,8,9})
};

std::vector<TileMapResource> tileMapResources = {
	TileMapResource("example_tilemap.txt",resource::SpriteSheet::levelsTileset,sf::Vector2u(64,64),sf::Vector2u(64,64),{0,1,2,3,4})
};

std::vector<std::string> fontResources = {
	"arial.ttf"
};

std::vector<std::string> soundResources = {
	"punch1.wav",
	"punch2.wav"
};

const sf::IntRect RegularSSB::getTextureRect(int index)const{
	int x,y = 0;
	if(index<getIndexEnd()){
		x = index%columns;
		y = (index-x)/columns;
	}
	return sf::IntRect(x*tileWidth,y*tileHeight,tileWidth,tileHeight);
}

const sf::IntRect IrregularSSB::getTextureRect(int index)const{
	if(index>=0&&index<tiles.size()){
		return tiles[index];
	}else{
		if(tiles.size()!=0){
			return tiles[0];
		}else{
			return sf::IntRect(0,0,0,0);
		}
	}
}

const sf::Vector2f IrregularSSB::getTileSize(int index)const{
	if(index>=0&&index<tiles.size()){
		return sf::Vector2f(tiles[index].width,tiles[index].height);
	}else{
		if(tiles.size()!=0){
			return sf::Vector2f(tiles[0].width,tiles[0].height);
		}else{
			return sf::Vector2f(0,0);
		}
	}
}

const SpriteSheet& Resources::getSpriteSheet(resource::SpriteSheet spriteSheet)const{
	int index = static_cast<int>(spriteSheet);
	if(index<0 || index>=spriteSheets.size()){
		return spriteSheets[0];
	}else{
		return spriteSheets[index];
	}
}

const Animation& Resources::getAnimation(resource::Animation animation)const{
	int index = static_cast<int>(animation);
	if(index<0 || index>=animations.size()){
		return animations[0];
	}else{
		return animations[index];
	}
}

const TileMapData& Resources::getTileMapData(resource::TileMap tileMap)const{
	int index = static_cast<int>(tileMap);
	if(index<0 || index>tilemapData.size()){
		return tilemapData[0];
	}else{
		return tilemapData[index];
	}
}

const sf::Font& Resources::getFont(resource::Font font)const{
	int index = static_cast<int>(font);
	if(index<0 || index>=fonts.size()){
		return fonts[0].font;
	}else{
		return fonts[index].font;
	}
}

const sf::SoundBuffer& Resources::getSoundBuffer(resource::Sound sound)const{
	int index = static_cast<int>(sound);
	if(index<0 || index>=sounds.size()){
		return sounds[0].soundBuffer;
	}else{
		return sounds[index].soundBuffer;
	}
}

TileMapData::TileMapData(const std::string& name, const SpriteSheet& spriteSheet, const std::vector<int> tiles,
	const sf::Vector2u size, const sf::Vector2u tileSize, const std::vector<int> solidTiles) :
	name(name), spriteSheet(spriteSheet), tiles(tiles), size(size), tileSize(tileSize), tileCount(0), solidTiles(solidTiles) {
	for (std::vector<int>::const_iterator iter = tiles.begin(); iter != tiles.end(); iter++) {
		if (*iter != -1) {
			tileCount++;
		}
	}
}

const int TileMapData::getTileFrameIndex(int x, int y)const {
	int index = y * size.x + x;
	if (x < 0 || x >= size.x || y < 0 || y >= size.y) {
		return -1;
	}
	else {
		return tiles[y*size.x + x];
	}
}
const bool TileMapData::isSolid(int frameIndex)const {
	std::vector<int>::const_iterator iter = std::find(solidTiles.begin(), solidTiles.end(), frameIndex);
	return iter != solidTiles.end();
}
const bool TileMapData::isSolid(int x, int y)const {
	return isSolid(getTileFrameIndex(x, y));
}

void jumpTo(std::ifstream& file,std::string label){
	std::string line;
	do{
		std::getline(file,line);
	}while(line!=label&&file);
}

void getArgs(const std::string& line,std::vector<std::string>& args){
	args.clear();
	std::stringstream parser(line);
	std::string arg;
	while(parser>>arg){
		args.push_back(arg);
	}
}

bool loadResources(Resources& resources){

	sf::Texture texture;
	for(std::vector<SpriteSheetResource>::const_iterator it = spriteSheetResources.begin(); it!=spriteSheetResources.end(); it++){

		if(!texture.loadFromFile("resources/images/"+it->imageName)){
			std::cout << "Could not read file: " << it->imageName << std::endl;
			return false;
		}

		if(it->configuration.size()==2){
			resources.spriteSheets.push_back(SpriteSheet(it->imageName,texture,it->configuration[0],it->configuration[1]));
		}else if(it->configuration.size()%4==0){
			std::vector<sf::IntRect> tiles;
			for(int i=0;i<it->configuration.size();i+=4){
				tiles.push_back(sf::IntRect(it->configuration[i],it->configuration[i+1],it->configuration[i+2],it->configuration[i+3]));
			}
			resources.spriteSheets.push_back(SpriteSheet(it->imageName,texture,tiles));
		}else{
			std::cout << "Invalid spritesheet configuation with image " << it->imageName << std::endl;
		}
	}
	
	sf::Font font;
	for(std::vector<std::string>::const_iterator it = fontResources.begin(); it!=fontResources.end(); it++){
		if (!font.loadFromFile("resources/fonts/" + *it)) {
			std::cout << "Couldn't load font" << *it << std::endl;
			return false;
		}
		resources.fonts.push_back(FontData(*it, font));
	}
	
	resources.animations = animationResources;

	for(std::vector<TileMapResource>::const_iterator it = tileMapResources.begin(); it!=tileMapResources.end(); it++){
		
		std::vector<int> tiles;
		
		std::ifstream tilemapFile;
		tilemapFile.open(("resources/tilemaps/" + it->name).c_str());
		if (!tilemapFile.is_open()) {
			std::cout << "Couldn't open tilemap file " << it->name << std::endl;
			return false;
		}

		int height = 0;
		std::string line;
		std::vector<std::string> args;
		while (std::getline(tilemapFile, line)) {
			getArgs(line, args);
			std::transform(args.begin(), args.end(), std::back_inserter(tiles), stringToInt);
			if (args.size() < it->size.x) {
				for (int i = 0; i < it->size.x - args.size(); i++) {
					tiles.push_back(0);
				}
			}
			height++;
		}
		tilemapFile.close();

		resources.tilemapData.push_back(TileMapData(it->name, resources.getSpriteSheet(it->spriteSheet),
			tiles, sf::Vector2u(it->size.x,height), it->tileSize, it->solidTiles));
	}
	
	sf::SoundBuffer soundBuffer;
	for(std::vector<std::string>::const_iterator it = soundResources.begin(); it!=soundResources.end(); it++){
		if(!soundBuffer.loadFromFile("resources/sounds/"+*it)){
			std::cout<<"Couldn't read file: "<<*it<<std::endl;
			return false;
		}
		resources.sounds.push_back(SoundData(*it,soundBuffer));
	} 
	
	return true;
}
