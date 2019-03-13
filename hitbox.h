#ifndef HITBOX_H_GUARD
#define HITBOX_H_GUARD

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"

class TileMap;

struct HitboxType{
	enum{
		NONE,
		CIRCLE, //scales by the max of the abs of scale.x,scale.y
		RECT, //stays axis aligned
		POLYGON, //calculated roughBounds at start, then simply checks collision with boundingRect of the rotation of this rect, then checks the exact polygon collision.
		POLYGON_RECT //roughBounds encompases the full rotation of the vertices and scales by the max of the abs of scale.x,scale.y.
					 //if roughBounds collides, it checks if the transformable has a different scale and/or rotation from last time and recomputes the exact bounds
					 //then checks collision with this rect.
	};
};

struct PolyData{
	std::vector<sf::Vector2f> vertices;
	float rotation;
	sf::Vector2f scale;
	sf::FloatRect exactBounds;
	
	PolyData(const std::vector<sf::Vector2f>& vertices):vertices(vertices),rotation(0),scale(),exactBounds(){}
	PolyData(const std::vector<sf::Vector2f>& vertices,float rotation,const sf::Vector2f& scale):vertices(vertices),rotation(rotation),scale(scale),exactBounds(){}
};
		
//The roughBounds for a hitbox is guaranteed to contain the hitbox, ie: if there isn't a collision with the rough bounds, then there isn't a collision with the hitbox.
//This is done to improve performance, there is no point doing accurate collision checks between every entity when some are far apart.
//However, the hitboxes are transformed differently as described in the enum above. The getHitboxTransform function in collidable.cpp gives the necessary transfrom
//required to transform the roughBounds to check collisions.

class Hitbox{
public:
	Hitbox():type(HitboxType::NONE),roughBounds(){}
	Hitbox(float radius):type(HitboxType::CIRCLE),roughBounds(-radius,-radius,2*radius,2*radius),radius(radius){}
	Hitbox(float x,float y,float width,float height):type(HitboxType::RECT),roughBounds(x,y,width,height){}
	Hitbox(const std::vector<sf::Vector2f>& vertices):type(HitboxType::POLYGON),roughBounds(),polyData(vertices){ setPolygonRoughBounds(); }
	Hitbox(const std::vector<sf::Vector2f>& vertices,float rotation,const sf::Vector2f& scale):type(HitboxType::POLYGON_RECT),roughBounds(),polyData(vertices,rotation,scale){
			setPolygonRectRoughBounds(); setPolygonRectExactBounds(); }
	
	const int getType()const{ return type; }
	const sf::FloatRect& getRoughBounds()const{ return roughBounds; }
	
	const float getRadius()const{ return radius; }
	const std::vector<sf::Vector2f>& getVertices()const{ return polyData.vertices; }
	const float getRotation()const{ return polyData.rotation; }
	const sf::Vector2f& getScale()const{ return polyData.scale; }
	const sf::FloatRect& getExactBounds()const{ return polyData.exactBounds; }
	const PolyData& getPolyData()const{ return polyData; }
	
	void updatePolygonRect(float,const sf::Vector2f&);
		
	~Hitbox(){}
	Hitbox(const Hitbox& hitbox);
	Hitbox& operator=(const Hitbox&);
private:

	void setPolygonRoughBounds();
	void setPolygonRectRoughBounds();
	void setPolygonRectExactBounds();
	
	int type;
	sf::FloatRect roughBounds;
	
	union{
		float radius;
		PolyData polyData;
	};
};

void drawHitbox(sf::RenderTarget&,const sf::Transformable&,const Hitbox&);
const bool collide(const sf::Transformable&,Hitbox&,const sf::Transformable&,Hitbox&);
const bool collideTileMap(const TileMap&,const sf::Transformable&,Hitbox&);

#endif