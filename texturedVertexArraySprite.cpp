#include "texturedVertexArraySprite.h"

void TexturedVertexArraySprite::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	states.texture = texture;
	target.draw(vertices,states);
}