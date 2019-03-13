#ifndef TEXTURED_VERTEX_ARRAY_SPRITE_H_GUARD
#define TEXTURED_VERTEX_ARRAY_SPRITE_H_GUARD

#include "vertexArraySprite.h"

class TexturedVertexArraySprite: public VertexArraySprite{
public:
	TexturedVertexArraySprite(const sf::Texture& texture):VertexArraySprite(),texture(texture){}
	TexturedVertexArraySprite(const sf::Texture& texture,sf::PrimitiveType type,unsigned int vertexCount=0):VertexArraySprite(type,vertexCount),texture(texture){}
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;
	
protected:
	
	const sf::Texture& texture;
};

#endif