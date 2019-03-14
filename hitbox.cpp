#include "hitbox.h"
#include <algorithm>
#include <iostream>
#include "polygonSprite.h"
#include <cmath>

const float maxAbsScale(const sf::Vector2f& scale) {
	return std::max(abs(scale.x), abs(scale.y));
}
//The .getTransform() of transformables will transform around their origin, whereas the origin of the hitboxes
//is (0,0). This function returns a sf::Transform which takes this into account.
sf::Transform getHitboxTransform(const sf::Transformable& transformable, const Hitbox& hitbox, bool exact) { //exact is for POLYGON_RECT
	if (hitbox.getType() == HitboxType::CIRCLE) {
		float scalar = maxAbsScale(transformable.getScale());
		return (sf::Transform()).translate(transformable.getPosition()).scale(scalar, scalar);
	}
	else if (hitbox.getType() == HitboxType::RECT) {
		return (sf::Transform()).translate(transformable.getPosition()).scale(transformable.getScale());
	}
	else if (hitbox.getType() == HitboxType::POLYGON) {
		return (sf::Transform()).combine(transformable.getTransform()).translate(transformable.getOrigin());
	}
	else if (hitbox.getType() == HitboxType::POLYGON_RECT) {
		if (!exact) {
			float scalar = maxAbsScale(transformable.getScale());
			return (sf::Transform()).translate(transformable.getPosition()).scale(scalar, scalar);
		}
		else {
			return (sf::Transform()).translate(transformable.getPosition());
		}
	}
}

void Hitbox::setPolygonRoughBounds(){
	if(getVertices().size()>0){
		float minX=getVertices()[0].x, maxX=getVertices()[0].x;
		float minY=getVertices()[0].y, maxY=getVertices()[0].y;
		for(std::vector<sf::Vector2f>::const_iterator iter=getVertices().begin()+1;iter!=getVertices().end();iter++){
			minX = std::min(minX,iter->x);
			maxX = std::max(maxX,iter->x);
			minY = std::min(minY,iter->y);
			maxY = std::max(maxY,iter->y);
		}
		roughBounds = sf::FloatRect(minX,minY,maxX-minX,maxY-minY);
	}
}

void Hitbox::setPolygonRectRoughBounds(){
	if(getVertices().size()>0){
		float max = hypot(getVertices()[0].x,getVertices()[0].y);
		for(std::vector<sf::Vector2f>::const_iterator iter=getVertices().begin()+1;iter!=getVertices().end();iter++){
			max = std::max(max,(float)hypot(iter->x,iter->y));
		}
		roughBounds = sf::FloatRect(-max,-max,max*2,max*2);
	}
}

void Hitbox::setPolygonRectExactBounds(){
	if(getVertices().size()>0){
		sf::Transform transform = (sf::Transform()).rotate(getRotation()).scale(getScale());
		sf::Vector2f point = transform.transformPoint(getVertices()[0]);
		float minX=point.x, maxX=point.x;
		float minY=point.y, maxY=point.y;
		for(std::vector<sf::Vector2f>::const_iterator iter=getVertices().begin()+1;iter!=getVertices().end();iter++){
			point = transform.transformPoint(*iter);
			minX = std::min(minX,point.x);
			maxX = std::max(maxX,point.x);
			minY = std::min(minY,point.y);
			maxY = std::max(maxY,point.y);
		}
		polyData.exactBounds = sf::FloatRect(minX,minY,maxX-minX,maxY-minY);
	}
}

void Hitbox::updatePolygonRect(float newRotation,const sf::Vector2f& scale){
	bool changed = false;
	if(abs(getRotation()-newRotation)>2){
		polyData.rotation = newRotation;
		changed=true;
	}
	if(abs(scale.x-getScale().x)<0.05 || abs(scale.y-getScale().y)<0.05){
		polyData.scale = scale;
		changed=true;
	}
	if(changed)
		setPolygonRectExactBounds();
}

Hitbox::Hitbox(const Hitbox& copy):type(copy.getType()),roughBounds(copy.getRoughBounds()){
	switch(type){
		case HitboxType::CIRCLE:
			radius = copy.getRadius();
			break;
		case HitboxType::POLYGON:
			polyData = copy.getPolyData();
			break;
		case HitboxType::POLYGON_RECT:
			polyData = copy.getPolyData();
			break;
	}
}

Hitbox& Hitbox::operator=(const Hitbox& copy){
	type=copy.getType();
	roughBounds=copy.getRoughBounds();
	switch(type){
		case HitboxType::CIRCLE:
			radius = copy.getRadius();
			break;
		case HitboxType::POLYGON:
			polyData = copy.getPolyData();
			break;
		case HitboxType::POLYGON_RECT:
			polyData = copy.getPolyData();
			break;
	}
	return *this;
}

//Funcion for drawing hitboxes

void drawHitbox(sf::RenderTarget& target, const sf::Transformable& transformable, const Hitbox& hitbox) {

	sf::RectangleShape rect(sf::Vector2f(hitbox.getRoughBounds().width, hitbox.getRoughBounds().height));
	rect.setOrigin(-hitbox.getRoughBounds().left, -hitbox.getRoughBounds().top);
	rect.setPosition(transformable.getPosition());

	if (hitbox.getType() != HitboxType::POLYGON_RECT && hitbox.getType() != HitboxType::CIRCLE) {
		rect.setScale(transformable.getScale());
	}
	else {
		float scalar = maxAbsScale(transformable.getScale());
		rect.setScale(scalar, scalar);
	}
	if (hitbox.getType() == HitboxType::POLYGON) {
		rect.setRotation(transformable.getRotation());
	}
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(5);
	rect.setFillColor(sf::Color::Transparent);
	target.draw(rect);

	if (hitbox.getType() == HitboxType::POLYGON) {
		sf::FloatRect boundingRect = getHitboxTransform(transformable, hitbox).transformRect(hitbox.getRoughBounds());
		sf::RectangleShape boundingRectShape(sf::Vector2f(boundingRect.width, boundingRect.height));
		boundingRectShape.setPosition(boundingRect.left, boundingRect.top);
		boundingRectShape.setOutlineColor(sf::Color(0, 255, 0));
		boundingRectShape.setOutlineThickness(1);
		boundingRectShape.setFillColor(sf::Color::Transparent);
		target.draw(boundingRectShape);
	}
	if (hitbox.getType() == HitboxType::POLYGON_RECT) {
		sf::FloatRect boundingRect = (sf::Transform()).translate(transformable.getPosition()).transformRect(hitbox.getExactBounds());
		sf::RectangleShape boundingRectShape(sf::Vector2f(boundingRect.width, boundingRect.height));
		boundingRectShape.setPosition(boundingRect.left, boundingRect.top);
		boundingRectShape.setOutlineColor(sf::Color(0, 255, 0));
		boundingRectShape.setOutlineThickness(1);
		boundingRectShape.setFillColor(sf::Color::Transparent);
		target.draw(boundingRectShape);
	}
	if (hitbox.getType() == HitboxType::CIRCLE) {
		sf::CircleShape circle(hitbox.getRadius());
		circle.setOrigin(-hitbox.getRoughBounds().left, -hitbox.getRoughBounds().top);
		circle.setPosition(transformable.getPosition());
		float scalar = maxAbsScale(transformable.getScale());
		circle.setScale(scalar, scalar);
		circle.setOutlineColor(sf::Color(255, 0, 0));
		circle.setOutlineThickness(1);
		circle.setFillColor(sf::Color::Transparent);
		target.draw(circle);
	}
	else if (hitbox.getType() == HitboxType::POLYGON || hitbox.getType() == HitboxType::POLYGON_RECT) {
		PolygonSprite polygon(hitbox.getVertices());
		polygon.setPosition(transformable.getPosition());
		polygon.setScale(transformable.getScale());
		polygon.setRotation(transformable.getRotation());
		target.draw(polygon);
	}

}