
#include "compoundSprite.h"

void CompoundSprite::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	states.transform *= getTransform();
	for(std::vector<SpriteSheetSprite>::const_iterator it = sprites.begin(); it!=sprites.end(); it++){
		target.draw(*it, states);
	}
}

void CompoundSprite::addSprite(const SpriteSheet& spriteSheet){
	sprites.push_back(SpriteSheetSprite(spriteSheet));
}

void CompoundSprite::setFrame(int spriteIndex, int frame){
	if(spriteIndex<sprites.size()){
		sprites[spriteIndex].setFrame(frame);
	}
}