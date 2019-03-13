#include "spriteUSS.h"

SpriteUSS::SpriteUSS(const SpriteSheet& spriteSheet):sf::Sprite(spriteSheet.getTexture()),
		spriteSheetBounds(spriteSheet.getSpriteSheetBounds()){
	setOrigin(spriteSheetBounds.getTileSize()/2.f);
	setFrame(0);
}

void SpriteUSS::setSpriteSheet(const SpriteSheet& spriteSheet){
	setTexture(spriteSheet.getTexture());
	spriteSheetBounds = spriteSheet.getSpriteSheetBounds();
}

void SpriteUSS::setFrame(int index){
	setTextureRect(spriteSheetBounds.getTextureRect(index));
}
