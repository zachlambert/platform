#include "vertexArraySprite.h"

void VertexArraySprite::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(vertices,states);
}

