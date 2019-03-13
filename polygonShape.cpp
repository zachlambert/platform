#include "polygonShape.h"

PolygonShape::PolygonShape(const std::vector<float>& points,const sf::Color& color){
	vertices.setPrimitiveType(sf::LineStrip);
	vertices.resize(points.size()/2+1);
	for(std::vector<float>::size_type i=0;i<points.size()/2;i++){
		vertices[i].position=sf::Vector2f(points[i*2],points[i*2+1]);
		vertices[i].color = color;
	}
	vertices[points.size()/2].position=sf::Vector2f(points[0],points[1]);
	vertices[points.size()/2].color = color;
}

PolygonShape::PolygonShape(const std::vector<sf::Vector2f>& points,const sf::Color& color){
	vertices.setPrimitiveType(sf::LineStrip);
	vertices.resize(points.size()+1);
	for(std::vector<sf::Vector2f>::size_type i=0;i<points.size();i++){
		vertices[i].position = points[i];
		vertices[i].color = color;
	}
	vertices[points.size()].position = points[0];
	vertices[points.size()].color = color;
}