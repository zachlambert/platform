#ifndef POLYGON_SHAPE_H_GUARD
#define POLYGON_SHAPE_H_GUARD

#include "vertexArraySprite.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class PolygonShape: public VertexArraySprite{
public:
	PolygonShape(const std::vector<float>&,const sf::Color& = sf::Color::Red);
	PolygonShape(const std::vector<sf::Vector2f>&,const sf::Color& = sf::Color::Red);
};

#endif