#ifndef VERTEX_ARRAY_SPRITE_H_GUARD
#define VERTEX_ARRAY_SPRITE_H_GUARD

#include <SFML/Graphics.hpp>

class VertexArraySprite: public sf::Transformable, public sf::Drawable{
public:
	VertexArraySprite():vertices(){}
	VertexArraySprite(sf::PrimitiveType type,unsigned int vertexCount=0):vertices(type,vertexCount){}
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;
	
protected:
	
	sf::VertexArray vertices;

};

#endif