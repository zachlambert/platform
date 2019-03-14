#ifndef TILEMAP_SPRITE_H_GUARD
#define TILEMAP_SPRITE_H_GUARD

#include <vector>
#include "texturedVertexArraySprite.h"
#include "resources.h"

class SpriteSheet;

class TileMapData{
public:
	TileMapData(const std::string&,const SpriteSheet&,const std::vector<int>,const sf::Vector2u,const sf::Vector2u,const std::vector<int>);
	
	const std::string& getName()const{ return name; }
	const SpriteSheet& getSpriteSheet()const{ return spriteSheet; }
	const std::vector<int>& getTiles()const{ return tiles; }
	const int getTileFrameIndex(int x,int y)const;
	const sf::Vector2u& getSize()const{ return size; }
	const sf::Vector2u& getTileSize()const{ return tileSize; }
	const int getTileCount()const{ return tileCount; }
	const bool isSolid(int)const;
	const bool isSolid(int,int)const;
	
private:
	std::string name;
	const SpriteSheet& spriteSheet;
	std::vector<int> tiles;
	sf::Vector2u size;
	sf::Vector2u tileSize;
	int tileCount;
	std::vector<int> solidTiles;
};

class TileMapSprite: public TexturedVertexArraySprite{
public:
	TileMapSprite(const TileMapData&);

	const TileMapData& getTileMapData()const{ return tileMapData; }

private:

	const TileMapData& tileMapData;
};

#endif