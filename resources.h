#ifndef RESOURCES_H_GUARD
#define RESOURCES_H_GUARD

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tilemap.h"

class TileMapData;

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
	const std::string name;
	const float duration;
	const std::vector<int> frames;
	Animation(const std::string& name,float duration,const std::vector<int>& frames):name(name),duration(duration),frames(frames){}
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

class Resources{
public:
	std::vector<SpriteSheet> spriteSheets;
	std::vector<FontData> fonts;
	std::vector<Animation> animations;
	std::vector<TileMapData> tilemapData;
	std::vector<SoundData> sounds;
	
	const SpriteSheet& getSpriteSheet(const std::string&)const;
	const Animation& getAnimation(const std::string&)const;
	const TileMapData& getTileMapData(const std::string&)const;
	const sf::Font& getFont(const std::string&)const;
	const sf::SoundBuffer& getSoundBuffer(const std::string&)const;
};

bool loadResources(Resources&);

#endif