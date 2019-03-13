/* NEW ENTITY h//

### PURPOSE ###
-Defines NewEntity class (rename to Entity when implemented)

### NEW ENTITY INTERFACE ###

Constructor:
	Takes resources to store

getters:
	These are defined as return nullptr here, but will
	be overloaded in child classes, when they implement
	different funcionality
	-d	  | < These two may return a pointer to the same object. Would ideally just return a sprite, but some entities
	-t | < may use vertexArraySprite, which is a custom sprite, which extends drawable and transformable. 
	-m
	-h
	-a

Objects for these getters are classes, which may have some methods

Non-member functions are used to update entities instead of having internal update methods

Note: There are non-const and const methods of each of the s,t,d,m,h,a, because they need to be accessible for modification, and accessible in
const functions. For example, the draw function (overriding sf::Drawable draw), must be const, so all functions called in it must be const. Therefore,
if you want to access the transformable for position, you need to get a const pointer.

*/



#ifndef ENTITY_H_GUARD
#define ENTITY_H_GUARD

#include <SFML/Graphics.hpp>
#include "motion.h"
#include "hitbox.h"
#include "animator.h"

class Entity {
public:

	Entity() {}

	virtual sf::Sprite* s() = 0;
	virtual const sf::Sprite* s()const = 0;

	virtual sf::Drawable* d() = 0;
	virtual const sf::Drawable* d()const = 0;

	virtual sf::Transformable* t() = 0;
	virtual const sf::Transformable* t()const = 0;
	
	virtual Motion* m() = 0;
	virtual const Motion* m()const = 0;

	virtual Hitbox* h() = 0;
	virtual const Hitbox* h()const = 0;

	virtual Animator* a() = 0;
	virtual const Animator* a()const = 0;

	virtual void setFrame(int frame)=0;

};

void entityMove(Entity& entity, float seconds);
void entityMoveBack(Entity& entity, float seconds);
void entityUpdateAnimation(Entity& entity, float seconds);

#endif
