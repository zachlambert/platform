/* ENTITY h// */



#ifndef ENTITY_H_GUARD
#define ENTITY_H_GUARD

#include <SFML/Graphics.hpp>

#include "motion.h"
#include "hitbox.h"
#include "animator.h"

#include "spriteSheetSprite.h"
#include "compoundSprite.h"
#include "particleSystemSprite.h"
#include "tileMapSprite.h"

class Entity: public sf::Drawable{
public:

	Entity() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const { target.draw(d()); }
	virtual void update(float seconds) = 0;

	virtual sf::Transformable& t() = 0;
	virtual const sf::Transformable& t()const = 0;
	virtual const sf::Drawable& d()const = 0;
};


class PhysicalEntity: public Entity{
public:
	PhysicalEntity():hitbox(),motion(){}

	Hitbox& h() { return hitbox; }
	const Hitbox& h()const { return hitbox; }
	Motion& m() { return motion; }
	const Motion& m()const { return motion; }

	virtual void setOnGround() = 0;
	virtual void setOffGround() = 0;

protected:
	Hitbox hitbox;
	Motion motion;

};

//Instead of creating a class for each Entity/PhysicalEntity and Sprite combination, a template
//can be used. Note: By sprite, I mean anything which extends transformable and drawable.
template <class E, class S>
class EntityTemplate: public E{
public:
	EntityTemplate(S sprite):E(),sprite(sprite){}

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const { return sprite; }
	const sf::Drawable& d()const { return sprite; }

	S& s(){ return sprite; }
	const S& s()const{ return sprite; }

private:
	S sprite;
};

//This can just be inherited by anything that wants to keep track of an animation and 
//retrieve the current frame. It doesn't directly manipulate anything itself.
class Animated {
public:
	Animated(const Resources& resources) :animator(resources) {}

	Animator& a() { return animator; }
	const Animator& a()const { return animator; }

protected:
	Animator animator;
};


#endif
