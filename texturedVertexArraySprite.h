#ifndef TEXTURED_VERTEX_ARRAY_SPRITE_H_GUARD
#define TEXTURED_VERTEX_ARRAY_SPRITE_H_GUARD

#include "vertexArraySprite.h"

class TexturedVertexArraySprite: public VertexArraySprite{
public:
	TexturedVertexArraySprite():VertexArraySprite(),texture(0){}
	TexturedVertexArraySprite(const sf::Texture& texture):VertexArraySprite(),texture(&texture){}
	TexturedVertexArraySprite(const sf::Texture& texture,sf::PrimitiveType type,unsigned int vertexCount=0):VertexArraySprite(type,vertexCount),texture(&texture){}

	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;
	
protected:
	
	//Pointer is required instead of const reference, in order for it to be able to be copied
	const sf::Texture* texture; //Shallow copy is intended
};

#endif