#ifndef RESOURCES_H_GUARD
#define RESOURCES_H_GUARD

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "resourceNames.h"

class BaseSSB{
public:
	BaseSSB(){}
	virtual const sf::IntRect getTextureRect(int)const=0;
	virtual const int getIndexEnd()const=0;
	virtual const sf::Vector2f getTileSize(int=0)const=0;
	virtual ~BaseSSB(){}
	virtual BaseSSB* clone()const=0;
};

class RegularSSB: public BaseSSB{
public:
	RegularSSB():BaseSSB(),columns(0),rows(0),tileWidth(0),tileHeight(0){}
	RegularSSB(const sf::Texture& texture,int columns,int rows):BaseSSB(),columns(columns),rows(rows),
		tileWidth(texture.getSize().x/columns),tileHeight(texture.getSize().y/rows){}
	virtual const sf::IntRect getTextureRect(int)const;
	virtual const int getIndexEnd()const{ return columns*rows; }
	virtual const sf::Vector2f getTileSize(int=0)const{ return sf::Vector2f(tileWidth,tileHeight); }
	
	virtual BaseSSB* clone()const{ return new RegularSSB(*this); }
private:
	int columns;
	int rows;
	int tileWidth;
	int tileHeight;
};

class IrregularSSB: public BaseSSB{
public:
	IrregularSSB():BaseSSB(),tiles(){}
	IrregularSSB(const std::vector<sf::IntRect>& tiles):BaseSSB(),tiles(tiles){}
	virtual const sf::IntRect getTextureRect(int)const;
	virtual const int getIndexEnd()const{ return tiles.size(); }
	virtual const sf::Vector2f getTileSize(int=0)const;
	
	virtual BaseSSB* clone()const{ return new IrregularSSB(*this); }
private:
	std::vector<sf::IntRect> tiles;
};

class SpriteSheetBounds{
public:
	SpriteSheetBounds(const sf::Texture& texture,int columns,int rows):ssb(new RegularSSB(texture,columns,rows)){}
	SpriteSheetBounds(const std::vector<sf::IntRect>& tiles):ssb(new IrregularSSB(tiles)){}
		
	~SpriteSheetBounds(){ delete ssb; }
	SpriteSheetBounds(const SpriteSheetBounds& copy):ssb(copy.ssb->clone()){}
	SpriteSheetBounds& operator=(const SpriteSheetBounds& copy) { ssb = copy.ssb->clone(); return *this; }
	
	const sf::IntRect getTextureRect(int index)const{ return ssb->getTextureRect(index); }
	const int getIndexEnd()const{ return ssb->getIndexEnd(); }
	const sf::Vector2f getTileSize(int index=0)const{ return ssb->getTileSize(index); }
private:
	BaseSSB* ssb;
};

class SpriteSheet{
public:
	SpriteSheet(std::string name,sf::Texture texture,int columns,int rows):
		name(name),texture(texture),ssb(texture,columns,rows){}
	SpriteSheet(std::string name,sf::Texture texture,const std::vector<sf::IntRect>& tiles):
		name(name),texture(texture),ssb(tiles){}
	
	const std::string& getName()const{ return name; }
	const sf::Texture& getTexture()const{ return texture; }
	const SpriteSheetBounds& getSpriteSheetBounds()const{ return ssb; }
	const sf::IntRect getTextureRect(int index)const{ return ssb.getTextureRect(index);} //May not need this, remove later if I still don't need it.
private:
	const std::string name;
	const sf::Texture texture;
	SpriteSheetBounds ssb;
};

struct Animation{
	float duration;
	std::vector<int> frames;
	Animation(float duration,const std::vector<int>& frames):duration(duration),frames(frames){}
};

struct FontData{
	std::string name;
	sf::Font font;
	FontData(const std::string& name,const sf::Font& font):name(name),font(font){}
};

struct SoundData{
	std::string name;
	sf::SoundBuffer soundBuffer;
	SoundData(const std::string& name,const sf::SoundBuffer& soundBuffer):name(name),soundBuffer(soundBuffer){}
};

class TileMapData {
public:
	TileMapData(const SpriteSheet& spriteSheet, const sf::Vector2u tileSize, const sf::Vector2u size,
		    const std::vector<int> solidTiles, const std::vector<int> backgroundTiles, const std::vector<int> foregroundTiles) :
		spriteSheet(spriteSheet), tileSize(tileSize), size(size), solidTiles(solidTiles), backgroundTiles(backgroundTiles), foregroundTiles(foregroundTiles){}

	const SpriteSheet& getSpriteSheet()const { return spriteSheet; }

	const int getTileFrameIndex(const std::vector<int>&, int x, int y)const;
	const sf::Vector2u& getTileSize()const { return tileSize; }
	const sf::Vector2u& getSize()const { return size; }
	int getTileCount()const { return size.x*size.y; }

	const std::vector<int>& getSolidTiles()const { return solidTiles; }
	const std::vector<int>& getBackgroundTiles()const { return backgroundTiles; }
	const std::vector<int>& getForegroundTiles()const { return foregroundTiles; }

	bool isSolid(int x,int y)const;

private:
	const SpriteSheet& spriteSheet;
	sf::Vector2u tileSize;
	sf::Vector2u size;
	std::vector<int> solidTiles;
	std::vector<int> backgroundTiles;
	std::vector<int> foregroundTiles;
};

class Resources{
public:
	std::vector<SpriteSheet> spriteSheets;
	std::vector<FontData> fonts;
	std::vector<Animation> animations;
	std::vector<TileMapData> tilemapData;
	std::vector<SoundData> sounds;
	
	const SpriteSheet& getSpriteSheet(resource::SpriteSheet)const;
	const Animation& getAnimation(resource::Animation)const;
	const TileMapData& getTileMapData(resource::TileMap)const;
	const sf::Font& getFont(resource::Font)const;
	const sf::SoundBuffer& getSoundBuffer(resource::Sound)const;
};

bool loadResources(Resources&);

#endif