#include "spriteSheetSprite.h"

SpriteSheetSprite::SpriteSheetSprite(const SpriteSheet& spriteSheet):sf::Sprite(spriteSheet.getTexture()),
		spriteSheetBounds(spriteSheet.getSpriteSheetBounds()){
	setOrigin(spriteSheetBounds.getTileSize()/2.f);
	setFrame(0);
}

void SpriteSheetSprite::setSpriteSheet(const SpriteSheet& spriteSheet){
	setTexture(spriteSheet.getTexture());
	spriteSheetBounds = spriteSheet.getSpriteSheetBounds();
}

void SpriteSheetSprite::setFrame(int index){
	setTextureRect(spriteSheetBounds.getTextureRect(index));
}
