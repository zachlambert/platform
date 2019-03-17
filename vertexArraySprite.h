#ifndef VERTEX_ARRAY_SPRITE_H_GUARD
#define VERTEX_ARRAY_SPRITE_H_GUARD

#include <SFML/Graphics.hpp>

class VertexArraySprite: public sf::Transformable, public sf::Drawable{
public:
	VertexArraySprite():vertices(){}
	virtual ~VertexArraySprite(){}

	VertexArraySprite(sf::PrimitiveType type,unsigned int vertexCount=0):vertices(type,vertexCount){}
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;

protected:

	sf::VertexArray vertices;

	friend class TileMap;
	friend class ParticleSystem;
};

#endif