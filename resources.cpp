#include "resources.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "utility.h"

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

struct isSpriteSheetFunctor{
	isSpriteSheetFunctor(const std::string& desired):desired(desired){}
	bool operator()(const SpriteSheet& spriteSheet){
		return spriteSheet.getName()==desired;
	}
private:
	const std::string& desired;
};

const SpriteSheet& Resources::getSpriteSheet(const std::string& name)const{
	std::vector<SpriteSheet>::const_iterator search =
		std::find_if(spriteSheets.begin(),spriteSheets.end(),isSpriteSheetFunctor(name));
	if(search==spriteSheets.end()){
		return *spriteSheets.begin();
	}else{
		return *search;
	}
}

struct isAnimationFunctor{
	isAnimationFunctor(const std::string& desired):desired(desired){}
	bool operator()(const Animation& animation){
		return animation.name==desired;
	}
private:
	const std::string& desired;
};

const Animation& Resources::getAnimation(const std::string& name)const{
	std::vector<Animation>::const_iterator search =
		std::find_if(animations.begin(),animations.end(),isAnimationFunctor(name));
	if(search==animations.end()){
		return *animations.begin();
	}else{
		return *search;
	}
}

struct isTileMapDataFunctor{
	isTileMapDataFunctor(const std::string& desired):desired(desired){}
	bool operator()(const TileMapData& tileMapData){
		return tileMapData.getName()==desired;
	}
private:
	const std::string& desired;
};

const TileMapData& Resources::getTileMapData(const std::string& name)const{
	std::vector<TileMapData>::const_iterator search =
		std::find_if(tilemapData.begin(),tilemapData.end(),isTileMapDataFunctor(name));
	if(search==tilemapData.end()){
		return *tilemapData.begin();
	}else{
		return *search;
	}
}

struct isFontFunctor{
	isFontFunctor(const std::string& desired):desired(desired){}
	bool operator()(const FontData& fontData){
		return fontData.name==desired;
	}
private:
	const std::string& desired;
};

const sf::Font& Resources::getFont(const std::string& name)const{
	std::vector<FontData>::const_iterator search =
		std::find_if(fonts.begin(),fonts.end(),isFontFunctor(name));
	if(search==fonts.end()){
		return fonts.begin()->font;
	}else{
		return search->font;
	}
}

struct isSoundFunctor{
	isSoundFunctor(const std::string& desired):desired(desired){}
	bool operator()(const SoundData& soundData){
		return soundData.name==desired;
	}
private:
	const std::string& desired;
};

const sf::SoundBuffer& Resources::getSoundBuffer(const std::string& name)const{
	std::vector<SoundData>::const_iterator search =
		std::find_if(sounds.begin(),sounds.end(),isSoundFunctor(name));
	if(search==sounds.end()){
		return sounds.begin()->soundBuffer;
	}else{
		return search->soundBuffer;
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
	std::ifstream resourcesFile;
	resourcesFile.open("resources/resources.txt");
	if(!resourcesFile.is_open()){
		std::cout<<"Can't open resources.txt"<<std::endl;
		return false;
	}

	std::string line;
	std::vector<std::string> args;
	
	jumpTo(resourcesFile,"#images");
	{
	sf::Texture texture;
	while(std::getline(resourcesFile,line)){
		if(line.size()==0){
			break;
		}else{
			getArgs(line,args);
			std::string name = args[0];
			if(args.size()==3){
				int width = stringToInt(args[1]);
				int height = stringToInt(args[2]);
				if(!texture.loadFromFile("resources/images/"+name)){
					std::cout<<"Could not read file: "<<name<<std::endl;
					return false;
				}
				resources.spriteSheets.push_back(SpriteSheet(name,texture,width,height));
			}else{
				std::string configFileName = args[1];
				std::vector<sf::IntRect> tiles;

				std::ifstream configFile;
				configFile.open(("resources/spriteSheetConfigs/"+configFileName).c_str());
				if(!configFile.is_open()){
					std::cout<<"Couldn't read file: "<<configFileName<<std::endl;
					return false;
				}
				while(std::getline(configFile,line)){
					if(line.size()==0){
						break;
					}else{
						getArgs(line,args);
						int top = stringToInt(args[0]);
						int left = stringToInt(args[1]);
						int width = stringToInt(args[2]);
						int height = stringToInt(args[3]);
						tiles.push_back(sf::IntRect(top,left,width,height));
					}
				}
				configFile.close();
				
				if(!texture.loadFromFile("resources/images/"+name)){
					std::cout<<"Could not read file: "<<name<<std::endl;
					return false;
				}
				resources.spriteSheets.push_back(SpriteSheet(name,texture,tiles));
			}
		}
	}
	}
	
	jumpTo(resourcesFile,"#fonts");
	{
	sf::Font font;
	while(std::getline(resourcesFile,line)){
		if(line.size()==0){
			break;
		}else{
			//only one arg, line = name
			if(!font.loadFromFile("resources/fonts/"+line)){
				std::cout<<"Couldn't load font"<<line<<std::endl;
				return false;
			}
			resources.fonts.push_back(FontData(line,font));
		}
	}
	}
	
	jumpTo(resourcesFile,"#animations");
	{
	std::string name;
	std::vector<int> frames;
	float duration;
	while(std::getline(resourcesFile,line)){
		if(line.size()==0){
			break;
		}else{
			getArgs(line,args);
			name = args[0];
			duration = stringToFloat(args[1]);
			
			std::transform(args.begin()+2,args.end(),std::back_inserter(frames),stringToInt);
			
			resources.animations.push_back(Animation(name,duration,frames));
			frames.clear();
		}
	}
	}
	
	jumpTo(resourcesFile,"#tilemaps");
	{
	std::string name;
	std::string tilemapFileName;
	std::string spriteSheetName;
	std::vector<int> tiles;
	std::vector<int> solidTiles;
	while(std::getline(resourcesFile,line)){
		if(line.size()==0){
			break;
		}else{
			getArgs(line,args);
			name = args[0];
			tilemapFileName = args[1];
			spriteSheetName = args[2];
			int width = stringToInt(args[3]);
			int tileWidth = stringToInt(args[4]);
			int tileHeight = stringToInt(args[5]);
			std::transform(args.begin()+6,args.end(),std::back_inserter(solidTiles),stringToInt);
			
			std::ifstream tilemapFile;
			tilemapFile.open(("resources/tilemaps/"+tilemapFileName).c_str());
			if(!tilemapFile.is_open()){
				std::cout<<"Couldn't open tilemap file "<<tilemapFileName<<std::endl;
				return false;
			}
			
			int height=0;
			while(std::getline(tilemapFile,line)){
				getArgs(line,args);
				std::transform(args.begin(),args.end(),std::back_inserter(tiles),stringToInt);
				if(args.size()<width){
					for(int i=0;i<width-args.size();i++){
						tiles.push_back(0);
					}
				}
				height++;
			}
			tilemapFile.close();
			
			resources.tilemapData.push_back(TileMapData(name,resources.getSpriteSheet(spriteSheetName),
				tiles,sf::Vector2u(width,height),sf::Vector2u(tileWidth,tileHeight),solidTiles));
			
			tiles.clear();
			solidTiles.clear();
		}
	}
	}
	
	jumpTo(resourcesFile,"#sounds");
	{
	sf::SoundBuffer soundBuffer;
	while(std::getline(resourcesFile,line)){
		if(line.size()==0){
			break;
		}else{
			//only 1 arg, name = line
			if(!soundBuffer.loadFromFile("resources/sounds/"+line)){
				std::cout<<"Couldn't read file: "<<line<<std::endl;
			}
			resources.sounds.push_back(SoundData(line,soundBuffer));
		}
	}
	}
	
	//Would close automatically without this, but putting here for completeness.
	resourcesFile.close();
	
	return true;
}
