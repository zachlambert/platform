#ifndef COMPOUND_SPRITE_H_GUARD
#define COMPOUND_SPRITE_H_GUARD

#include <SFML/Graphics.hpp>
#include <vector>
#include "resources.h"
#include "spriteSheetSprite.h"

class CompoundSprite: public sf::Transformable, public sf::Drawable{
public:
	CompoundSprite(){}
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;

	void addSprite(const SpriteSheet& spriteSheet);
	void setFrame(int spriteIndex,int frame);

private:
	std::vector<SpriteSheetSprite> sprites;

};

#endif
