#ifndef SPRITE_USS_H_GUARD
#define SPRITE_USS_H_GUARD

#include <SFML/Graphics.hpp>
#include "resources.h"

class SpriteSheetSprite: public sf::Sprite{
public:
	SpriteSheetSprite(const SpriteSheet& spriteSheet);
	
	void setSpriteSheet(const SpriteSheet&);
	void setFrame(int);

private:
	SpriteSheetBounds spriteSheetBounds; //I'm making this a value instead of a const reference so that you can reassign the spritesheet later. The other way to do this is use a pointer,
										 //but the spriteSheetBounds object is small (4 ints) so I don't think this is worth the additional complexity.
};

#endif